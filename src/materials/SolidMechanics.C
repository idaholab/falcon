#include "SolidMechanics.h"

template<>
InputParameters validParams<SolidMechanics>()
{
  InputParameters params= validParams<PorousMedia>();
//common mechanical properties-----------------------------------------------------------------
  params.addParam<Real>("thermal_expansion",1.0e-6,"thermal expansion coefficient (1/K)");
  params.addParam<Real>("youngs_modulus",1.50e10,"in Pascal") ;
  params.addParam<Real>("poissons_ratio",0.2,"dimensionless");

  params.addParam<Real>("biot_coeff",1.0,"dimensionless");
  params.addParam<Real>("biot_modulus",2.5e10,"dimensionless");
  params.addParam<Real>("t_ref",293.15,"initial temperature");
//has smear crack model------------------------------------------------------------------------------
  params.addParam<bool>("has_crack",false,"switch for crack model");
//smear crack based on tensile failure
  params.addParam<std::string>("has_crack_method","tension","switch for cracking method");
  params.addParam<Real>("critical_crack_strain",1.0,"crack strain threshold");
//smear crack model based on Mohr-Coulomb failure
  params.addParam<Real>("cohesion",0.0,"Rock's Cohesion strength");
  params.addParam<Real>("friction_angle",0.1,"Rock's internal friction angle");
  
//stress/strain dependent permeability-----------------------------------------------------------------------------
  params.addParam<bool>("has_damage_couple_permeability",false,"switch for couple damage with porosity or not");
  params.addParam<Real>("damage_couple_permeability_coeff1",2.0,"the first coeff for coupling damage with porosity");
  params.addParam<Real>("damage_couple_permeability_coeff2",1.0e5,"the second coeff for coupling damage with porosity");

//solid mechanics material model coupled with heat, pressure, and displacements--------------------------------------
  params.addCoupledVar("temperature", "TODO:  add description");
  params.addCoupledVar("pressure", "TODO:  add description");
  params.addCoupledVar("x_disp", "TODO: ad description");
  params.addCoupledVar("y_disp", "TODO: ad description");
  params.addCoupledVar("z_disp", "TODO: ad description");

  return params;

}
//////////////////////////////////////////////////////////////////////////





SolidMechanics::SolidMechanics(const std::string & name,
                               InputParameters parameters)
  :PorousMedia(name, parameters),
   _has_temp(isCoupled("temperature")),
   _grad_temp  (_has_temp ? coupledGradient("temperature") : _grad_zero),
   _temperature(_has_temp ? coupledValue("temperature")  : _zero),
   _has_x_disp(isCoupled("x_disp")),
   _grad_x_disp(_has_x_disp ? coupledGradient("x_disp") : _grad_zero),
   _has_y_disp(isCoupled("y_disp")),
   _grad_y_disp(_has_y_disp ? coupledGradient("y_disp") : _grad_zero),
   _has_z_disp(isCoupled("z_disp")),
   _grad_z_disp(_has_z_disp ? coupledGradient("z_disp") : _grad_zero),

   _input_thermal_expansion(getParam<Real>("thermal_expansion")),
   _input_youngs_modulus(getParam<Real>("youngs_modulus")),
   _input_poissons_ratio(getParam<Real>("poissons_ratio")),
   _input_biot_coeff(getParam<Real>("biot_coeff")),
   _input_biot_modulus(getParam<Real>("biot_modulus")),
   _input_t_ref(getParam<Real>("t_ref")),

   _has_crack(getParam<bool>("has_crack")),
   _has_crack_method(getParam<std::string>("has_crack_method")),
   _critical_crack_strain(getParam<Real>("critical_crack_strain")),
   _cohesion(getParam<Real>("cohesion")),
   _friction_angle(getParam<Real>("friction_angle")),
   _has_damage_couple_permeability(getParam<bool>("has_damage_couple_permeability")),
   _damage_couple_permeability_coeff1(getParam<Real>("damage_couple_permeability_coeff1")),
   _damage_couple_permeability_coeff2(getParam<Real>("damage_couple_permeability_coeff2")),
   
   _total_strain (LIBMESH_DIM,LIBMESH_DIM),
   _total_stress (LIBMESH_DIM,LIBMESH_DIM),
   _total_stress1(LIBMESH_DIM,LIBMESH_DIM),

   //declare material properties
   _thermal_strain(declareProperty<Real>("thermal_strain")),
   _alpha(declareProperty<Real>("alpha")),
   _youngs_modulus(declareProperty<Real>("youngs_modulus")),
   _poissons_ratio(declareProperty<Real>("poissons_ratio")),
   _biot_coeff(declareProperty<Real>("biot_coeff")),
   _biot_modulus(declareProperty<Real>("biot_modulus")),
   _damage_coeff(declareProperty<Real>("damage_coeff")),
   _damage_coeff_old(declarePropertyOld<Real>("damage_coeff")),

   _stress_normal_vector(declareProperty<RealVectorValue>("stress_normal_vector")),
   _stress_shear_vector (declareProperty<RealVectorValue>("stress_shear_vector")),
   _strain_normal_vector(declareProperty<RealVectorValue>("strain_normal_vector")),
   _strain_shear_vector (declareProperty<RealVectorValue>("strain_shear_vector")),
   _pstress_normal_vector(declareProperty<RealVectorValue>("pstress_normal_vector")),
   _pstrain_normal_vector(declareProperty<RealVectorValue>("pstrain_normal_vector")),
   _crack_flags(NULL),
   _crack_flags_old(NULL)

{
if ( _has_crack && _critical_crack_strain > 0)
  {
    _crack_flags     = &declareProperty<RealVectorValue>("crack_flags");
    _crack_flags_old = &declarePropertyOld<RealVectorValue>("crack_flags");
  }
}
//////////////////////////////////////////////////////////////////////////

void
SolidMechanics::computeProperties()
{
  PorousMedia::computeProperties();
  if (_t_step == 1  && _critical_crack_strain > 0)
  {
    for (unsigned int i = 0; i< _qrule->n_points(); ++i)
    {
      _damage_coeff[i]     = 0.0;
      _damage_coeff_old[i] = 0.0;
          
      for (unsigned int j = 0; j<LIBMESH_DIM; ++j)
      {
        (*_crack_flags)[i](j)     = 1.0;
        (*_crack_flags_old)[i](j) = 1.0;
      }
    }
  }

  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
//    std::cout<< "dimension"<<LIBMESH_DIM<<" "<<LIBMESH_DIM<<"\n";
    
    _youngs_modulus[qp] = _input_youngs_modulus;
      
    _alpha[qp]            = _input_thermal_expansion;

    if(_has_temp)
      _thermal_strain[qp] = _input_thermal_expansion*(_temperature[qp] - _input_t_ref);
    else
      _thermal_strain[qp] = 0.0;
      
    _poissons_ratio[qp]   = _input_poissons_ratio;
    _biot_coeff[qp]       = _input_biot_coeff;
    _biot_modulus[qp]     = _input_biot_modulus;

// first try for strain and stress vectors
    if (_has_x_disp && _has_y_disp)
    {
      _E = _youngs_modulus[qp];
      _nu =  _poissons_ratio[qp];
      _c1 = _E*(1.-_nu)/(1.+_nu)/(1.-2.*_nu);
      _c2 = _nu/(1.-_nu);
      _c3 = 0.5*(1.-2.*_nu)/(1.-_nu);

      _strain_normal_vector[qp](0) = _grad_x_disp[qp](0); //s_xx
      _strain_normal_vector[qp](1) = _grad_y_disp[qp](1); //s_yy
      if (LIBMESH_DIM == 3)
        _strain_normal_vector[qp](2) = _grad_z_disp[qp](2); //s_zz

      _strain_shear_vector[qp](0) = 0.5*(_grad_x_disp[qp](1)+_grad_y_disp[qp](0)); // s_xy

      if (LIBMESH_DIM == 3)
      {
        _strain_shear_vector[qp](1) = 0.5*(_grad_x_disp[qp](2)+_grad_z_disp[qp](0)); // s_xz
        _strain_shear_vector[qp](2) = 0.5*(_grad_y_disp[qp](2)+_grad_z_disp[qp](1)); // s_yz
      }

        _stress_normal_vector[qp](0) = _c1*_strain_normal_vector[qp](0)+_c1*_c2*_strain_normal_vector[qp](1)+_c1*_c2*_strain_normal_vector[qp](2); //tau_xx
        _stress_normal_vector[qp](1) = _c1*_c2*_strain_normal_vector[qp](0)+_c1*_strain_normal_vector[qp](1)+_c1*_c2*_strain_normal_vector[qp](2); //tau_yy
        if (LIBMESH_DIM == 3)
          _stress_normal_vector[qp](2) = _c1*_c2*_strain_normal_vector[qp](0)+_c1*_c2*_strain_normal_vector[qp](1)+_c1*_strain_normal_vector[qp](2); //tau_zz

        _stress_shear_vector[qp](0) = _c1*_c3*2.0*_strain_shear_vector[qp](0); //tau_xy
        if (LIBMESH_DIM == 3)
        {
          _stress_shear_vector[qp](1) = _c1*_c3*2.0*_strain_shear_vector[qp](1); //tau_xz
          _stress_shear_vector[qp](2) = _c1*_c3*2.0*_strain_shear_vector[qp](2); //tau_yz
        }
    }

//smear crack model   
   if (_has_crack)
   {
      
      if( _has_crack_method == "tension") computeCrack_tension(qp);     //smeared crack model : tension induced crack
      if( _has_damage_couple_permeability  && _damage_coeff[qp] > 1.0e-5)
      {
       _permeability[qp] = _input_permeability*_damage_couple_permeability_coeff2;
       std::cout<< "modifying permeability q"<<qp<<" "<< _damage_coeff[qp] <<""<<_permeability[qp] <<"\n";
      }
      
   }

  }

}

void
SolidMechanics::computeCrack_tension(const int qp)
{

  _total_strain(0,0)= _strain_normal_vector[qp](0); //sxx
  _total_strain(1,1)= _strain_normal_vector[qp](1); //syy
  if (LIBMESH_DIM == 3) _total_strain(2,2)= _strain_normal_vector[qp](2); //szz
  _total_strain(0,1) = _strain_shear_vector[qp](0); //sxy
  _total_strain(1,0) = _strain_shear_vector[qp](0); //syx
  if (LIBMESH_DIM == 3)
  {
    _total_strain(0,2) = _strain_shear_vector[qp](1); //sxz
    _total_strain(2,0) = _strain_shear_vector[qp](1); //szx
    _total_strain(1,2) = _strain_shear_vector[qp](2); //syz
    _total_strain(2,1) = _strain_shear_vector[qp](2); //szy
  }

  _total_stress(0,0)= _stress_normal_vector[qp](0);
  _total_stress(1,1)= _stress_normal_vector[qp](1);
  if (LIBMESH_DIM == 3) _total_stress(2,2)= _stress_normal_vector[qp](2);
  _total_stress(0,1) = _stress_shear_vector[qp](0);
  _total_stress(1,0) = _stress_shear_vector[qp](0);
  if (LIBMESH_DIM == 3)
  {
    _total_stress(0,2) = _stress_shear_vector[qp](1);
    _total_stress(2,0) = _stress_shear_vector[qp](1);
    _total_stress(1,2) = _stress_shear_vector[qp](2);
    _total_stress(2,1) = _stress_shear_vector[qp](2);
  }

  _total_stress1(0,0)= _stress_normal_vector[qp](0);
  _total_stress1(1,1)= _stress_normal_vector[qp](1);
  if (LIBMESH_DIM == 3) _total_stress1(2,2)= _stress_normal_vector[qp](2);
  _total_stress1(0,1) = _stress_shear_vector[qp](0);
  _total_stress1(1,0) = _stress_shear_vector[qp](0);
  if (LIBMESH_DIM == 3)
  {
    _total_stress1(0,2) = _stress_shear_vector[qp](1);
    _total_stress1(2,0) = _stress_shear_vector[qp](1);
    _total_stress1(1,2) = _stress_shear_vector[qp](2);
    _total_stress1(2,1) = _stress_shear_vector[qp](2);
  }


  const int ND = LIBMESH_DIM;
  ColumnMajorMatrix e_vec(ND,ND);
  ColumnMajorMatrix principal_strain(ND,1);
  ColumnMajorMatrix principal_stress(ND,1);

//  _total_strain.eigen( principal_strain, e_vec );
  

  const Real tiny(1.0e-8);
  principal_strain(0,0) = _total_strain(0,0);
  principal_strain(1,0) = _total_strain(1,1);
  principal_strain(2,0) = _total_strain(2,2);
  
//  std::cout<< "total_strain "<<_total_strain(0,0)<<" "<<_total_strain(1,1)<<" "<<_total_strain(2,2)<<"\n";
//  std::cout<< "princ_strain "<<principal_strain(0,0)<<" "<<principal_strain(1,0)<<" "<<principal_strain(2,0)<<"\n";
  
  for (unsigned int i(0); i < LIBMESH_DIM; ++i)
  {

    if (principal_strain(i,0) > _critical_crack_strain)
    {
      (*_crack_flags)[qp](i) = tiny;
      _damage_coeff[qp]     =  1.0;
      std::cout << "***************crack initiated****************";
      std::cout<< "crack_direction"<< i+1 <<"\n";
    }
    else
    {
      (*_crack_flags)[qp](i) = 1.0;

    }

    (*_crack_flags)[qp](i) = std::min((*_crack_flags)[qp](i), (*_crack_flags_old)[qp](i));//once cracked, always cracked-no cohension

    _damage_coeff[qp] = std::max(_damage_coeff_old[qp] , _damage_coeff[qp]);


  }

  RealVectorValue crack_flags( (*_crack_flags)[qp] );
  for (unsigned int i(0); i < LIBMESH_DIM; ++i)
  {
    if (principal_strain(i,0) < 0)
    {
      crack_flags(i) = 1;  //even crack opens, under compression, it still holds
    }
    else
    {
      crack_flags(i) = (*_crack_flags)[qp](i);
    }
  }

  ColumnMajorMatrix e_vec1(ND,ND);
//  _total_stress1.eigen( principal_stress, e_vec1 );

  for (unsigned int i(0); i < LIBMESH_DIM; ++i)
  {
    if (crack_flags(i) < 0.5) //aleady cracked and now under tension
    {
//      principal_stress(i,0) = tiny;  //zero out the principal stress perpendicular to the fracture
      for(unsigned j(0);j<LIBMESH_DIM;j++)
      {
        _total_stress(i,j) = tiny;
        _total_stress(j,i) = tiny;
      }
      
      
      std::cout<< "crack_direction"<< i+1 <<"\n";
    }
  }

//   ColumnMajorMatrix trans(ND,ND);

//   for (unsigned int j(0); j < LIBMESH_DIM; ++j)
//   {
//     for (unsigned int i(0); i < LIBMESH_DIM; ++i)
//     {
//       trans(i,j) = e_vec1(i,j);
//       _total_stress(i,j) = 0.0;
//       if(i == j)   _total_stress(i,j) = principal_stress(i,0);
//     }
//   }

// back rotate the modified principal stress tensor into origional coordinate
//  rotateSymmetricTensor( trans, _total_stress, _total_stress);

  _stress_normal_vector[qp](0)=_total_stress(0,0);
  _stress_normal_vector[qp](1)=_total_stress(1,1);
  if (LIBMESH_DIM == 3) _stress_normal_vector[qp](2)=_total_stress(2,2);
  
  _stress_shear_vector[qp](0)=_total_stress(0,1) ;
  if (LIBMESH_DIM == 3)
  {
    _stress_shear_vector[qp](1) = _total_stress(0,2);
    _stress_shear_vector[qp](2) = _total_stress(1,2) ;
  }

}
//////////////////////////////////////////////////////////////////////////





void
SolidMechanics::rotateSymmetricTensor( const ColumnMajorMatrix & R,
                                      const RealTensorValue & T,
                                      RealTensorValue & result )
{

  //     R           T         Rt
  //  00 01 02   00 01 02   00 10 20
  //  10 11 12 * 10 11 12 * 01 11 21
  //  20 21 22   20 21 22   02 12 22
  //
  const Real T00 = R(0,0)*T(0,0) + R(0,1)*T(1,0) + R(0,2)*T(2,0);
  const Real T01 = R(0,0)*T(0,1) + R(0,1)*T(1,1) + R(0,2)*T(2,1);
  const Real T02 = R(0,0)*T(0,2) + R(0,1)*T(1,2) + R(0,2)*T(2,2);

  const Real T10 = R(1,0)*T(0,0) + R(1,1)*T(1,0) + R(1,2)*T(2,0);
  const Real T11 = R(1,0)*T(0,1) + R(1,1)*T(1,1) + R(1,2)*T(2,1);
  const Real T12 = R(1,0)*T(0,2) + R(1,1)*T(1,2) + R(1,2)*T(2,2);

  const Real T20 = R(2,0)*T(0,0) + R(2,1)*T(1,0) + R(2,2)*T(2,0);
  const Real T21 = R(2,0)*T(0,1) + R(2,1)*T(1,1) + R(2,2)*T(2,1);
  const Real T22 = R(2,0)*T(0,2) + R(2,1)*T(1,2) + R(2,2)*T(2,2);

  result = RealTensorValue(
    T00 * R(0,0) + T01 * R(0,1) + T02 * R(0,2),
    T00 * R(1,0) + T01 * R(1,1) + T02 * R(1,2),
    T00 * R(2,0) + T01 * R(2,1) + T02 * R(2,2),

      T10 * R(0,0) + T11 * R(0,1) + T12 * R(0,2),
      T10 * R(1,0) + T11 * R(1,1) + T12 * R(1,2),
      T10 * R(2,0) + T11 * R(2,1) + T12 * R(2,2),

      T20 * R(0,0) + T21 * R(0,1) + T22 * R(0,2),
      T20 * R(1,0) + T21 * R(1,1) + T22 * R(1,2),
      T20 * R(2,0) + T21 * R(2,1) + T22 * R(2,2) );
  
}
//////////////////////////////////////////////////////////////////////////

