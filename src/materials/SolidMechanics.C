#include "SolidMechanics.h"

template<>
InputParameters validParams<SolidMechanics>()
{
  InputParameters params= validParams<PorousMedia>();
  params.addParam<Real>("thermal_expansion",1.0e-6,"thermal expansion coefficient (1/K)");
  params.addParam<Real>("youngs_modulus",1.50e10,"in Pascal") ;
  params.addParam<Real>("poissons_ratio",0.2,"dimensionless");
  params.addParam<Real>("biot_coeff",0.0,"dimensionless");
  params.addParam<Real>("t_ref",293.15,"initial temperature");
// damage related parameters
  params.addParam<bool>("has_damage",false,"switch for turning on/off damaging mechanics");
  params.addParam<Real>("damage_coeff",0.0,"initial damage value");
  params.addParam<Real>("strain_initialize_damage",0.01,"critical strain to initialize damage");
  params.addParam<Real>("strain_broken",0.04,"critical strain to complete failure");
  params.addParam<Real>("input_damage_a1",1.0,"1st parameter of Damage evolution");
  params.addParam<Real>("input_damage_a2",2.0,  "2nd parameter of Damage evolution");
//has crack model
  params.addParam<bool>("has_crack",false,"switch for crack model");
  params.addParam<std::string>("has_crack_method","tension","switch for cracking method");
  params.addParam<Real>("critical_crack_strain",1.0,"crack strain threshold");
//  params.addParam<RealVectorValue>("initial crack flag",(1.0,1.0,1.0),"crack flag");
  params.addParam<Real>("cohesion",0.0,"Rock's Cohesion strength");
  params.addParam<Real>("friction_angle",0.1,"Rock's internal friction angle");
  
  
  params.addParam<Real>("t_ref",293.15,"initial temperature");
  params.addCoupledVar("temperature", "TODO:  add description");
  params.addCoupledVar("x_disp", "TODO: ad description");
  params.addCoupledVar("y_disp", "TODO: ad description");
  params.addCoupledVar("z_disp", "TODO: ad description");

  return params;
  
}

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
   _input_t_ref(getParam<Real>("t_ref")),

   _has_damage(getParam<bool>("has_damage")),
   _input_damage_coeff(getParam<Real>("damage_coeff")),
   _input_strain_initialize_damage(getParam<Real>("strain_initialize_damage")),
   _input_strain_broken(getParam<Real>("strain_broken")),
   _damage_a1(getParam<Real>("input_damage_a1")),
   _damage_a2(getParam<Real>("input_damage_a2")),

   _has_crack(getParam<bool>("has_crack")),
   _has_crack_method(getParam<std::string>("has_crack_method")),
   _critical_crack_strain(getParam<Real>("critical_crack_strain")),
   _cohesion(getParam<Real>("cohesion")),
   _friction_angle(getParam<Real>("friction_angle")),
   
   _total_strain (LIBMESH_DIM,LIBMESH_DIM),
   _total_stress (LIBMESH_DIM,LIBMESH_DIM),
   _total_stress1(LIBMESH_DIM,LIBMESH_DIM),
   //declare material properties
   _thermal_strain(declareProperty<Real>("thermal_strain")),
   _alpha(declareProperty<Real>("alpha")),
   _youngs_modulus(declareProperty<Real>("youngs_modulus")),
   _poissons_ratio(declareProperty<Real>("poissons_ratio")),
   _biot_coeff(declareProperty<Real>("biot_coeff")),
   _damage_coeff(declareProperty<Real>("damage_coeff")),
   _strain_history(declareProperty<Real>("strain_history")),
   
   _stress_normal_vector(declareProperty<RealVectorValue>("stress_normal_vector")),
   _stress_shear_vector (declareProperty<RealVectorValue>("stress_shear_vector")),
   _strain_normal_vector(declareProperty<RealVectorValue>("strain_normal_vector")),
   _strain_shear_vector (declareProperty<RealVectorValue>("strain_shear_vector")),
   _crack_flags(NULL),
   _crack_flags_old(NULL)
{
  if ( _has_crack && _critical_crack_strain > 0)
  {
    _crack_flags = &declareProperty<RealVectorValue>("crack_flags");
    _crack_flags_old = &declarePropertyOld<RealVectorValue>("crack_flags");
  }
}

void
SolidMechanics::computeProperties()
{
  PorousMedia::computeProperties();
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
    _alpha[qp]            = _input_thermal_expansion;
    if(_has_temp)
      _thermal_strain[qp] = _input_thermal_expansion*(_temperature[qp] - _input_t_ref);
    else
      _thermal_strain[qp] = 0.0;
    _youngs_modulus[qp]   = _input_youngs_modulus;
    
    _poissons_ratio[qp]   = _input_poissons_ratio;
    _biot_coeff[qp]       = _input_biot_coeff;
    
    if (_has_damage) computeDamage(qp); //by damage mechanics theory
    
    if (_has_x_disp && _has_y_disp)
    {
      _E  =  _youngs_modulus[qp];//newly modified
      _nu =  _poissons_ratio[qp];
      _c1 = _E*(1.-_nu)/(1.+_nu)/(1.-2.*_nu);
      _c2 = _nu/(1.-_nu);
      _c3 = 0.5*(1.-2.*_nu)/(1.-_nu);
      
      _strain_normal_vector[qp](0) = _grad_x_disp[qp](0); //s_xx
      _strain_normal_vector[qp](1) = _grad_y_disp[qp](1); //s_yy
      if (_dim == 3)
        _strain_normal_vector[qp](2) = _grad_z_disp[qp](2); //s_zz

      _strain_shear_vector[qp](0) = 0.5*(_grad_x_disp[qp](1)+_grad_y_disp[qp](0)); // s_xy

      if (_dim == 3)
      {
        _strain_shear_vector[qp](1) = 0.5*(_grad_x_disp[qp](2)+_grad_z_disp[qp](0)); // s_xz
        _strain_shear_vector[qp](2) = 0.5*(_grad_y_disp[qp](2)+_grad_z_disp[qp](1)); // s_yz
      }
      
      _stress_normal_vector[qp](0) = _c1*_strain_normal_vector[qp](0)+_c1*_c2*_strain_normal_vector[qp](1)+_c1*_c2*_strain_normal_vector[qp](2); //tau_xx
      _stress_normal_vector[qp](1) = _c1*_c2*_strain_normal_vector[qp](0)+_c1*_strain_normal_vector[qp](1)+_c1*_c2*_strain_normal_vector[qp](2); //tau_yy
      if (_dim == 3)
        _stress_normal_vector[qp](2) = _c1*_c2*_strain_normal_vector[qp](0)+_c1*_c2*_strain_normal_vector[qp](1)+_c1*_strain_normal_vector[qp](2); //tau_zz

      _stress_shear_vector[qp](0) = _c1*_c3*2.0*_strain_shear_vector[qp](0); //tau_xy
      if (_dim == 3)
      {
        _stress_shear_vector[qp](1) = _c1*_c3*2.0*_strain_shear_vector[qp](1); //tau_xz
        _stress_shear_vector[qp](2) = _c1*_c3*2.0*_strain_shear_vector[qp](2); //tau_yz
      }
      
    }
    
    if (_has_crack)
    {
      if (_t_step == 1) // && _critical_crack_strain > 0)
      {
        for (unsigned int i = 0; i< _qrule->n_points(); ++i)
        {
          for (unsigned int j = 0; j<LIBMESH_DIM; ++j)
          {
            (*_crack_flags)[i](j)     = 1.0;
            (*_crack_flags_old)[i](j) = 1.0;
          }
        }
      }
      if( _has_crack_method == "tension") computeCrack_tension(qp);     //smeared crack model : tension induced crack
      if( _has_crack_method =="Mohr-Coulomb") computeCrack_Mohr_Coulomb(qp); //smeared crack model : Mohr-Coulomb criteria
    }
  }
    
}

void
SolidMechanics::computeDamage(const int qp)
{
  Real _effective_strain=0.0 , _temp =0.0;                 //newly added
  _strain_history[qp]   = std::max(_input_strain_initialize_damage,_strain_history[qp]);
  _damage_coeff[qp]     = std::max(_input_damage_coeff, _damage_coeff[qp]);
  if(_dim==2)
  {
//        _effective_strain = std::max(std::abs(_grad_x_disp[qp](0)) ,std::abs( _grad_y_disp[qp](1)));
    _temp = (_grad_x_disp[qp](0) + _grad_y_disp[qp](1))/2.;
    _effective_strain = (_grad_x_disp[qp](0)-_temp)*(_grad_x_disp[qp](0)-_temp) +
      (_grad_y_disp[qp](1)-_temp)*(_grad_y_disp[qp](1)-_temp) +
      (_grad_x_disp[qp](1)+_grad_y_disp[qp](0))*(_grad_x_disp[qp](1)+_grad_y_disp[qp](0))/2.0;
  }
  if(_dim==3)
  {
//        _effective_strain = std::max(std::abs(_grad_x_disp[qp](0)) ,std::abs( _grad_y_disp[qp](1)));
//        _effective_strain = std::max(std::abs(_grad_z_disp[qp](2)) , _effective_strain);
    _temp = (_grad_x_disp[qp](0)+_grad_y_disp[qp](1)+_grad_z_disp[qp](2))/3.;
    _effective_strain = (_grad_x_disp[qp](0)-_temp)*(_grad_x_disp[qp](0)-_temp) +
          (_grad_y_disp[qp](1)-_temp)*(_grad_y_disp[qp](1)-_temp) +
          (_grad_z_disp[qp](2)-_temp)*(_grad_z_disp[qp](2)-_temp) +
          (_grad_x_disp[qp](1)+_grad_y_disp[qp](0))*(_grad_x_disp[qp](1)+_grad_y_disp[qp](0))/2.0 +
          (_grad_x_disp[qp](2)+_grad_z_disp[qp](0))*(_grad_x_disp[qp](2)+_grad_z_disp[qp](0))/2.0 +
          (_grad_y_disp[qp](2)+_grad_z_disp[qp](1))*(_grad_y_disp[qp](2)+_grad_z_disp[qp](1))/2.0 ;
  }
    
  _effective_strain = std::pow(_effective_strain*2./3. , 0.5);
      
  if(_strain_history[qp] < _input_strain_broken) //not fully failed
  {
    if(_effective_strain <= _strain_history[qp])//no further damaging
    {
      _damage_coeff[qp]   = _damage_coeff[qp];
      _strain_history[qp] = _strain_history[qp];
    }
    else //damaging continues
    {
      if(_effective_strain >= _input_strain_broken)//fully failed: s > sc
      {
        _damage_coeff[qp]   = 0.999;
        _strain_history[qp] = _effective_strain;
      }
      else//continuously damaging s-{s0,sc}
      {
        _temp = (_effective_strain - _input_strain_initialize_damage)/(_input_strain_broken - _input_strain_initialize_damage);
        _temp = _damage_a1 * std::pow(_temp , _damage_a2) + _input_damage_coeff;
        if(_temp >= 0.999)
        {
          std::cout<<"The parameter of damage evolution are too large,please change them"<<"\n";
          _temp = 0.999;
        }
        _damage_coeff[qp] = std::max(_temp , _damage_coeff[qp]);
        _strain_history[qp] = _effective_strain;
      }
    }
  }
  else //alreadry completely failed
  {
    _damage_coeff[qp] = 0.999;
    _strain_history[qp] =  std::max(_strain_history[qp] , _effective_strain);
  }
  
  _youngs_modulus[qp] = (1.0-_damage_coeff[qp])*_input_youngs_modulus;
}

void
SolidMechanics::computeCrack_tension(const int qp)
{
  _total_strain(0,0)= _strain_normal_vector[qp](0);
  _total_strain(1,1)= _strain_normal_vector[qp](1);
  if (_dim == 3) _total_strain(2,2)= _strain_normal_vector[qp](2);
  _total_strain(0,1) = _strain_shear_vector[qp](0);
  _total_strain(1,0) = _strain_shear_vector[qp](0);
  if (_dim == 3)
  {
    _total_strain(0,2) = _strain_shear_vector[qp](1);
    _total_strain(2,0) = _strain_shear_vector[qp](1);
    _total_strain(1,2) = _strain_shear_vector[qp](2);
    _total_strain(2,1) = _strain_shear_vector[qp](2);    
  }
  
  _total_stress(0,0)= _stress_normal_vector[qp](0);
  _total_stress(1,1)= _stress_normal_vector[qp](1);
  if (_dim == 3) _total_stress(2,2)= _stress_normal_vector[qp](2);
  _total_stress(0,1) = _stress_shear_vector[qp](0);
  _total_stress(1,0) = _stress_shear_vector[qp](0);
  if (_dim == 3)
  {
    _total_stress(0,2) = _stress_shear_vector[qp](1);
    _total_stress(2,0) = _stress_shear_vector[qp](1);
    _total_stress(1,2) = _stress_shear_vector[qp](2);
    _total_stress(2,1) = _stress_shear_vector[qp](2);    
  }

  _total_stress1(0,0)= _stress_normal_vector[qp](0);
  _total_stress1(1,1)= _stress_normal_vector[qp](1);
  if (_dim == 3) _total_stress1(2,2)= _stress_normal_vector[qp](2);
  _total_stress1(0,1) = _stress_shear_vector[qp](0);
  _total_stress1(1,0) = _stress_shear_vector[qp](0);
  if (_dim == 3)
  {
    _total_stress1(0,2) = _stress_shear_vector[qp](1);
    _total_stress1(2,0) = _stress_shear_vector[qp](1);
    _total_stress1(1,2) = _stress_shear_vector[qp](2);
    _total_stress1(2,1) = _stress_shear_vector[qp](2);    
  }

//update crack_flag before getting into calculation
//  for (unsigned int i(0); i < LIBMESH_DIM; ++i)
//  {
//    (*_crack_flags_old)[qp](i) = (*_crack_flags)[qp](i);
//  }
  
  const int ND = LIBMESH_DIM;
  ColumnMajorMatrix e_vec(ND,ND);
  ColumnMajorMatrix principal_strain(ND,1);
  ColumnMajorMatrix principal_stress(ND,1);
  _total_strain.eigen( principal_strain, e_vec );
  
  const Real tiny(1.0e-8);
  
  for (unsigned int i(0); i < LIBMESH_DIM; ++i)
  {
    if (principal_strain(i,0) > _critical_crack_strain)
    {
      (*_crack_flags)[qp](i) = tiny;
      _damage_coeff[qp]     =  1.0;
    }
    else
    {
      (*_crack_flags)[qp](i) = 1.0;
    }
    
    (*_crack_flags)[qp](i) = std::min((*_crack_flags)[qp](i), (*_crack_flags_old)[qp](i));
    
  }

  RealVectorValue crack_flags( (*_crack_flags)[qp] );
  for (unsigned int i(0); i < LIBMESH_DIM; ++i)
  {
    if (principal_strain(i,0) < 0)
    {
      crack_flags(i) = 1;
    }
    else
    {
      crack_flags(i) = (*_crack_flags)[qp](i);
    }
}

  ColumnMajorMatrix e_vec1(ND,ND);
  _total_stress1.eigen( principal_stress, e_vec1 );
  
  for (unsigned int i(0); i < LIBMESH_DIM; ++i)
  {
    if (crack_flags(i) < 0.5)
    {
      principal_stress(i,0) = tiny;
    }
  }
  
ColumnMajorMatrix trans(LIBMESH_DIM,LIBMESH_DIM);
  
  for (unsigned int j(0); j < LIBMESH_DIM; ++j)
  {
    for (unsigned int i(0); i < LIBMESH_DIM; ++i)
    {
      trans(i,j) = e_vec1(i,j);
      _total_stress(i,j) = 0.0;
      if(i == j)   _total_stress(i,j) = principal_stress(i,0);
    }
  }

// back rotate the modified principal stress tensor into origional coordinate   
  rotateSymmetricTensor( trans, _total_stress, _total_stress);
  
  _stress_normal_vector[qp](0)=_total_stress(0,0);
  _stress_normal_vector[qp](1)=_total_stress(1,1);
  if (_dim == 3) _stress_normal_vector[qp](2)=_total_stress(2,2);
  _stress_shear_vector[qp](0)=_total_stress(0,1) ;
  if (_dim == 3)
  {
    _stress_shear_vector[qp](1) = _total_stress(0,2);
    _stress_shear_vector[qp](2) = _total_stress(1,2) ;
  }

}

////////////////////////////////////////////////////////////////////////

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
  if (LIBMESH_DIM == 3)
  {
    
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
  else if (LIBMESH_DIM == 2)
  {
    const Real T00 = R(0,0)*T(0,0) + R(0,1)*T(1,0);
    const Real T01 = R(0,0)*T(0,1) + R(0,1)*T(1,1) ;
    
    const Real T10 = R(1,0)*T(0,0) + R(1,1)*T(1,0) ;
    const Real T11 = R(1,0)*T(0,1) + R(1,1)*T(1,1) ;
    
    result = RealTensorValue(
      T00 * R(0,0) + T01 * R(0,1),
      T00 * R(1,0) + T01 * R(1,1),
      
      T10 * R(0,0) + T11 * R(0,1),
      T10 * R(1,0) + T11 * R(1,1));
  }
  
}



void
SolidMechanics::computeCrack_Mohr_Coulomb(const int qp)
{
  _total_stress(0,0)= _stress_normal_vector[qp](0);
  _total_stress(1,1)= _stress_normal_vector[qp](1);
  if (_dim == 3) _total_stress(2,2)= _stress_normal_vector[qp](2);
  _total_stress(0,1) = _stress_shear_vector[qp](0);
  _total_stress(1,0) = _stress_shear_vector[qp](0);
  if (_dim == 3)
  {
    _total_stress(0,2) = _stress_shear_vector[qp](1);
    _total_stress(2,0) = _stress_shear_vector[qp](1);
    _total_stress(1,2) = _stress_shear_vector[qp](2);
    _total_stress(2,1) = _stress_shear_vector[qp](2);    
  }

  _total_stress1(0,0)= _stress_normal_vector[qp](0);
  _total_stress1(1,1)= _stress_normal_vector[qp](1);
  if (_dim == 3) _total_stress1(2,2)= _stress_normal_vector[qp](2);
  _total_stress1(0,1) = _stress_shear_vector[qp](0);
  _total_stress1(1,0) = _stress_shear_vector[qp](0);
  if (_dim == 3)
  {
    _total_stress1(0,2) = _stress_shear_vector[qp](1);
    _total_stress1(2,0) = _stress_shear_vector[qp](1);
    _total_stress1(1,2) = _stress_shear_vector[qp](2);
    _total_stress1(2,1) = _stress_shear_vector[qp](2);    
  }

 //update crack_flag before getting into calculation
 // for (unsigned int i(0); i < LIBMESH_DIM; ++i)
 // {
 //   (*_crack_flags_old)[qp](i) = (*_crack_flags)[qp](i);
 // }
  
  const int ND = LIBMESH_DIM;
  ColumnMajorMatrix e_vec(ND,ND);
  ColumnMajorMatrix norm_vect(ND,1);
  ColumnMajorMatrix force(ND,1);
  Real temp;
  ColumnMajorMatrix principal_stress(ND,1);
  const Real tiny(1.0e-8);
  
  _total_stress1.eigen( principal_stress, e_vec );


    int ind_max = 0;
    int ind_min = 0;
    int ind_mid    ;
    Real s_max = principal_stress(0,0);
    Real s_min = principal_stress(0,0);

// find out minimum and maximum principal stress    
    for(int i = 1 ; i < LIBMESH_DIM ; ++i)
    {
      if(principal_stress(i,0) > s_max)
      {
        s_max = principal_stress(i,0);
        ind_max = i;
      }
      if(principal_stress(i,0) < s_min)
      {
        s_min = principal_stress(i,0);
        ind_min = i;
      }
    }
    for(int i=0 ; i < 3 ; ++i ) if(i != ind_max && i != ind_min) ind_mid = i;
    
    
    Real shear_temp = std::abs( principal_stress(ind_max,0) - principal_stress(ind_min,0) )/2.0;
    Real eigen_temp = _cohesion - _friction_angle * ( principal_stress(ind_max,0) + principal_stress(ind_min,0) ) / 2.0;
    if(shear_temp >= eigen_temp) //match Mohr_Coulomb Criteria, change the flags
    {
      (*_crack_flags)[qp](0) = tiny;
      _damage_coeff[qp] = 1.0;
    }
    else
    {
      (*_crack_flags)[qp](0) = 1.0;
    }
    
    (*_crack_flags)[qp](0) = std::min((*_crack_flags)[qp](0), (*_crack_flags_old)[qp](0));
  
//    if(shear_temp >= eigen_temp && (*_crack_flags)[qp](0) < 0.5) //if cracked, need to release shear stress
    if( (*_crack_flags)[qp](0) < 0.5) //if cracked, need to release shear stress    
    {
      if (LIBMESH_DIM == 3)
      {
        norm_vect(0) = (e_vec(0,ind_max) + e_vec(0,ind_min)) / std::pow(2.0,0.5);
        norm_vect(1) = (e_vec(1,ind_max) + e_vec(1,ind_min)) / std::pow(2.0,0.5);
        norm_vect(2) = (e_vec(2,ind_max) + e_vec(2,ind_min)) / std::pow(2.0,0.5);
      }
      else if (LIBMESH_DIM == 2)
      {
        norm_vect(0) = (e_vec(0,ind_max) + e_vec(0,ind_min)) / std::pow(2.0,0.5);
        norm_vect(1) = (e_vec(1,ind_max) + e_vec(1,ind_min)) / std::pow(2.0,0.5);
      }
      
      
      //get the complementary variables
      temp = 0.0;
      for(int i = 0 ; i < LIBMESH_DIM ; i++) 
      {
        force(i,0) = 0.0;
        for(int j = 0 ; j < LIBMESH_DIM ; j++)
        {
          force(i,0) += _total_stress1(i,j) * norm_vect(j) ;
        }
        
        temp += force(i,0) * norm_vect(i);
      }

      for(int i = 0 ; i < LIBMESH_DIM; i++)
      {
        for(int j = 0 ; j < LIBMESH_DIM ; j++)
        {
          
          _total_stress(i,j) =  temp * norm_vect(i)  * norm_vect(j) ;
//              _total_stress(i,j) =  force(i,0)  * norm_vect(j) ;


        }
      }
    }

//retrun modified stress tensor to specified form   
  _stress_normal_vector[qp](0)=_total_stress(0,0);
  _stress_normal_vector[qp](1)=_total_stress(1,1);
  if (_dim == 3) _stress_normal_vector[qp](2)=_total_stress(2,2);
  _stress_shear_vector[qp](0)=_total_stress(0,1) ;
  if (_dim == 3)
  {
    _stress_shear_vector[qp](1) = _total_stress(0,2);
    _stress_shear_vector[qp](2) = _total_stress(1,2) ;
  }

}

////////////////////////////////////////////////////////////////////////
