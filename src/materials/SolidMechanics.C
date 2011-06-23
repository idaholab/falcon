#include "SolidMechanics.h"

template<>
InputParameters validParams<SolidMechanics>()
{
  InputParameters params= validParams<PorousMedia>();

  params.addParam<Real>("thermal_expansion",1.0e-6,"thermal expansion coefficient (1/K)");
  params.addParam<Real>("youngs_modulus",1.50e10,"in Pascal") ;
  params.addParam<Real>("poissons_ratio",0.2,"dimensionless");

  params.addParam<Real>("biot_coeff",1.0,"dimensionless");
  params.addParam<Real>("t_ref",293.15,"initial temperature");
   
//damage related parameters
  params.addParam<bool>("has_damage",false,"switch for turning on/off damaging mechanics");
  params.addParam<Real>("damage_coeff",0.0,"initial damage value");
  params.addParam<Real>("strain_initialize_damage",0.01,"critical strain to initialize damage");
  params.addParam<Real>("strain_broken",0.04,"critical strain to complete failure");
  params.addParam<Real>("input_damage_a1",1.0,"1st parameter of Damage evolution");
  params.addParam<Real>("input_damage_a2",2.0,  "2nd parameter of Damage evolution");

//has anisotropic damage
  params.addParam<std::string>("has_damage_method","isotropic","switch of damage methods");
  params.addParam<Real>("input_damage_c",0.0,"the first parameter for anisotropic damage");
  params.addParam<Real>("input_damage_n",1.0,"the second parameter for anisotropic damage");
  params.addParam<Real>("input_strain_init",0.001,"the initial strain of damaging");
  params.addParam<Real>("input_strain_strength",0.0,"the third parameter for anisotropic damage");

//has crack model
  params.addParam<bool>("has_crack",false,"switch for crack model");
  params.addParam<std::string>("has_crack_method","tension","switch for cracking method");
  params.addParam<Real>("critical_crack_strain",1.0,"crack strain threshold");

  params.addParam<Real>("cohesion",0.0,"Rock's Cohesion strength");
  params.addParam<Real>("friction_angle",0.1,"Rock's internal friction angle");

  params.addCoupledVar("temperature", "TODO:  add description");
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
   _input_t_ref(getParam<Real>("t_ref")),

   _has_damage(getParam<bool>("has_damage")),
   _input_damage_coeff(getParam<Real>("damage_coeff")),
   _input_strain_initialize_damage(getParam<Real>("strain_initialize_damage")),
   _input_strain_broken(getParam<Real>("strain_broken")),
   _damage_a1(getParam<Real>("input_damage_a1")),
   _damage_a2(getParam<Real>("input_damage_a2")),

   _has_damage_method(getParam<std::string>("has_damage_method")),
   _input_damage_c(getParam<Real>("input_damage_c")),
   _input_damage_n(getParam<Real>("input_damage_n")),
   _input_strain_init(getParam<Real>("input_strain_init")),
   _input_strain_strength(getParam<Real>("input_strain_strength")),

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

   _damage_coeff_old(declarePropertyOld<Real>("damage_coeff")),
   _strain_history_old(declarePropertyOld<Real>("strain_history")),

   _stress_normal_vector(declareProperty<RealVectorValue>("stress_normal_vector")),
   _stress_shear_vector (declareProperty<RealVectorValue>("stress_shear_vector")),
   _strain_normal_vector(declareProperty<RealVectorValue>("strain_normal_vector")),
   _strain_shear_vector (declareProperty<RealVectorValue>("strain_shear_vector")),

   _init_status(false),
   _bond_nstiff(declareProperty<Real>("bond_nstiff")),
   _bond_sstiff(declareProperty<Real>("bond_sstiff")),

   _bond_damage_factor00 (declareProperty<Real>("bond_damage_factor00")),
   _bond_damage_factor00_old (declarePropertyOld<Real>("bond_damage_factor00")),

   _bond_damage_factor01 (declareProperty<Real>("bond_damage_factor01")),
   _bond_damage_factor01_old (declarePropertyOld<Real>("bond_damage_factor01")),

   _bond_damage_factor02 (declareProperty<Real>("bond_damage_factor02")),
   _bond_damage_factor02_old (declarePropertyOld<Real>("bond_damage_factor02")),

   _bond_damage_factor03 (declareProperty<Real>("bond_damage_factor03")),
   _bond_damage_factor03_old (declarePropertyOld<Real>("bond_damage_factor03")),

   _bond_damage_factor04 (declareProperty<Real>("bond_damage_factor04")),
   _bond_damage_factor04_old (declarePropertyOld<Real>("bond_damage_factor04")),

   _bond_damage_factor05 (declareProperty<Real>("bond_damage_factor05")),
   _bond_damage_factor05_old (declarePropertyOld<Real>("bond_damage_factor05")),

   _bond_damage_factor06 (declareProperty<Real>("bond_damage_factor06")),
   _bond_damage_factor06_old (declarePropertyOld<Real>("bond_damage_factor06")),

   _bond_damage_factor07 (declareProperty<Real>("bond_damage_factor07")),
   _bond_damage_factor07_old (declarePropertyOld<Real>("bond_damage_factor07")),

   _bond_damage_factor08 (declareProperty<Real>("bond_damage_factor08")),
   _bond_damage_factor08_old (declarePropertyOld<Real>("bond_damage_factor08")),

   _bond_damage_factor09 (declareProperty<Real>("bond_damage_factor09")),
   _bond_damage_factor09_old (declarePropertyOld<Real>("bond_damage_factor09")),

   _bond_damage_factor10 (declareProperty<Real>("bond_damage_factor10")),
   _bond_damage_factor10_old (declarePropertyOld<Real>("bond_damage_factor10")),

   _bond_damage_factor11 (declareProperty<Real>("bond_damage_factor11")),
   _bond_damage_factor11_old (declarePropertyOld<Real>("bond_damage_factor11")),

   _bond_damage_factor12 (declareProperty<Real>("bond_damage_factor12")),
   _bond_damage_factor12_old (declarePropertyOld<Real>("bond_damage_factor12")),


   _crack_flags(NULL),
   _crack_flags_old(NULL)

{
  _init_status = &declareProperty<bool>("init_status");
  if ( _has_crack && _critical_crack_strain > 0)
  {
    _crack_flags     = &declareProperty<RealVectorValue>("crack_flags");
    _crack_flags_old = &declarePropertyOld<RealVectorValue>("crack_flags");
  }

/*
  Real _input_thermal_expansion_temp = _input_thermal_expansion;
  Real _input_youngs_modulus_temp = _input_youngs_modulus;
  Real _input_poissons_ratio_temp = _input_poissons_ratio;
  Real _input_biot_coeff_temp = _input_biot_coeff;
  _input_thermal_expansion_temp *= 1.0 ;//(1.0 + 0.0 * (double(randn_trig()) - 0.5 ) );
  _input_youngs_modulus_temp    *= 1.0 ;//(1.0 + 0.0 * (double(randn_trig()) - 0.5 ) );
  _input_poissons_ratio_temp    *= 1.0 ;//(1.0 + 0.0 * (double(randn_trig()) - 0.5 ) );
  _input_biot_coeff_temp        *= 1.0 ;//(1.0 + 0.0 * (double(randn_trig()) - 0.5 ) );

  _alpha[_qp]            = _input_thermal_expansion_temp;
  if(_has_temp)
    _thermal_strain[_qp] = _input_thermal_expansion_temp*(_temperature[_qp] - _input_t_ref);
  else
  _thermal_strain[_qp] = 0.0;
  _youngs_modulus[_qp]   = _input_youngs_modulus_temp;
  _poissons_ratio[_qp]   = _input_poissons_ratio_temp;
  _biot_coeff[_qp]       = _input_biot_coeff_temp;
  if(LIBMESH_DIM==3)
  {
    _bond_nstiff[_qp] = 3.*_youngs_modulus[_qp]/(4.*3.14159265*(1.-2.*_poissons_ratio[_qp]));
    _bond_sstiff[_qp] = _bond_nstiff[_qp]*(1.-4*_poissons_ratio[_qp])/(1.+_poissons_ratio[_qp]);
  }
  if(LIBMESH_DIM==2)
  {
    // for plane-stress
    _bond_nstiff[_qp] = 2.*_youngs_modulus[_qp]/(3.14159265*(1.-_poissons_ratio[_qp]));
    _bond_sstiff[_qp] = _bond_nstiff[_qp]*(1.-3.*_poissons_ratio[_qp])/(1.+_poissons_ratio[_qp]);
    // for plane-strain
    //  _bond_nstiff[_qp] = 2.*_youngs_modulus[_qp]/3.14159265/(1.+_poissons_ratio[_qp])/(1.-2.*_poissons_ratio[_qp]);
    //  _bond_sstiff[_qp] = _bond_nstiff[_qp]*(1.-4.*_poissons_ratio[_qp])/2.;
  }
*/
}
//////////////////////////////////////////////////////////////////////////




void
SolidMechanics::computeProperties()
{
  PorousMedia::computeProperties();

  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
    if((*_init_status)[qp]==false)  //it induces that there are different values at different quadral points even in same element
    {
      (*_init_status)[qp] = true ;
      Real _input_thermal_expansion_temp = _input_thermal_expansion;
      Real _input_youngs_modulus_temp = _input_youngs_modulus;
      Real _input_poissons_ratio_temp = _input_poissons_ratio;
      Real _input_biot_coeff_temp = _input_biot_coeff;
      _input_thermal_expansion_temp *= 1.0 ;//(1.0 + 0.0 * (double(randn_trig()) - 0.5 ) );
      _input_youngs_modulus_temp    *= 1.0 ;//(1.0 + 0.0 * (double(randn_trig()) - 0.5 ) );
      _input_poissons_ratio_temp    *= 1.0 ;//(1.0 + 0.0 * (double(randn_trig()) - 0.5 ) );
      _input_biot_coeff_temp        *= 1.0 ;//(1.0 + 0.0 * (double(randn_trig()) - 0.5 ) );

      _alpha[qp]            = _input_thermal_expansion_temp;
      if(_has_temp)
        _thermal_strain[qp] = _input_thermal_expansion_temp*(_temperature[qp] - _input_t_ref);
      else
      _thermal_strain[qp] = 0.0;
      _youngs_modulus[qp]   = _input_youngs_modulus_temp;
      _poissons_ratio[qp]   = _input_poissons_ratio_temp;
      _biot_coeff[qp]       = _input_biot_coeff_temp;
      if(LIBMESH_DIM==3)
      {
        _bond_nstiff[qp] = 3.*_youngs_modulus[qp]/(4.*3.14159265*(1.-2.*_poissons_ratio[qp]));
        _bond_sstiff[qp] = _bond_nstiff[qp]*(1.-4*_poissons_ratio[qp])/(1.+_poissons_ratio[qp]);
      }
      if(LIBMESH_DIM==2)
      {
        // for plane-stress
        _bond_nstiff[qp] = 2.*_youngs_modulus[qp]/(3.14159265*(1.-_poissons_ratio[qp]));
        _bond_sstiff[qp] = _bond_nstiff[qp]*(1.-3.*_poissons_ratio[qp])/(1.+_poissons_ratio[qp]);
        // for plane-strain
        //  _bond_nstiff[qp] = 2.*_youngs_modulus[qp]/3.14159265/(1.+_poissons_ratio[qp])/(1.-2.*_poissons_ratio[qp]);
        //  _bond_sstiff[qp] = _bond_nstiff[qp]*(1.-4.*_poissons_ratio[qp])/2.;
      }
    }

    if (_has_damage && _has_damage_method == "isotropic") computeDamage(qp); //by damage mechanics theory
    
    if (_has_x_disp && _has_y_disp)
    {
      _E  =  _youngs_modulus[qp];//newly modified

//      if(_q_point[qp](2) < 0.5 && _q_point[qp](2) > -0.5 ) _E *= 0.9;//new added to prescribe the weak zone

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

      if(_has_damage == false || _has_damage_method != "anisotropic")//not need for anisotropic damage
      {
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

    }

    if(_has_damage && _has_damage_method == "anisotropic")   computeAnisoDamage(qp);    

    if (_has_crack)
    {
      if (_t_step == 1) // && _critical_crack_strain > 0)
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
      if( _has_crack_method == "tension") computeCrack_tension(qp);     //smeared crack model : tension induced crack
      if( _has_crack_method =="Mohr-Coulomb") computeCrack_Mohr_Coulomb_v2(qp); //smeared crack model : Mohr-Coulomb criteria
    }
  }
    
}
//////////////////////////////////////////////////////////////////////////




void
SolidMechanics::computeDamage(const int qp)
{
  Real _effective_strain=0.0 , _temp =0.0;                 //newly added
  _strain_history[qp]   = std::max(_input_strain_initialize_damage,_strain_history[qp]);
  _damage_coeff[qp]     = std::max(_input_damage_coeff, _damage_coeff[qp]);
  if(LIBMESH_DIM==2)
  {
//        _effective_strain = std::max(std::abs(_grad_x_disp[qp](0)) ,std::abs( _grad_y_disp[qp](1)));
    _temp = (_grad_x_disp[qp](0) + _grad_y_disp[qp](1))/2.;
    _effective_strain = (_grad_x_disp[qp](0)-_temp)*(_grad_x_disp[qp](0)-_temp) +
      (_grad_y_disp[qp](1)-_temp)*(_grad_y_disp[qp](1)-_temp) +
      (_grad_x_disp[qp](1)+_grad_y_disp[qp](0))*(_grad_x_disp[qp](1)+_grad_y_disp[qp](0))/2.0;
  }
  if(LIBMESH_DIM==3)
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

  //newly added to comply MOOSE procedure
  _damage_coeff[qp]   = std::max(_damage_coeff[qp] , _damage_coeff_old[qp]);
  _strain_history[qp] = std::max(_strain_history[qp] , _strain_history_old[qp]);

  if(_q_point[qp](0) > 0.8 && _q_point[qp](0) < 0.2 )  { _damage_coeff[qp] = 0.0 ; _strain_history[qp] = 0.0; } //newly added to avoid boundary damaging
  
  _youngs_modulus[qp] = (1.0-_damage_coeff[qp])*_input_youngs_modulus;
}
//////////////////////////////////////////////////////////////////////////



void
SolidMechanics::computeAnisoDamage(const int qp) //just calculate damage evolution according to strain not stress tensor
{
  _total_strain(0,0)= _strain_normal_vector[qp](0);
  _total_strain(1,1)= _strain_normal_vector[qp](1);
  if (LIBMESH_DIM == 3) _total_strain(2,2)= _strain_normal_vector[qp](2);
  _total_strain(0,1) = _strain_shear_vector[qp](0);
  _total_strain(1,0) = _strain_shear_vector[qp](0);
  if (LIBMESH_DIM == 3)
  {
    _total_strain(0,2) = _strain_shear_vector[qp](1);
    _total_strain(2,0) = _strain_shear_vector[qp](1);
    _total_strain(1,2) = _strain_shear_vector[qp](2);
    _total_strain(2,1) = _strain_shear_vector[qp](2);    
  }

  //space is divided into pieces with same spatial angle
  Real nstiff_tmp=0.0;
  Real sstiff_tmp=0.0;
  if(LIBMESH_DIM==3)
  {
    nstiff_tmp = _bond_nstiff[qp] * 2. * 3.14159265 / 13.;
    sstiff_tmp = _bond_sstiff[qp] * 2. * 3.14159265 / 13.;
//    sstiff_tmp = 0.0;
  }
  if(LIBMESH_DIM==2)
  {
    nstiff_tmp = _bond_nstiff[qp] * 3.14159265 / 12.;
    sstiff_tmp = _bond_sstiff[qp] * 3.14159265 / 12.;
//    sstiff_tmp = 0.0;
  }
  //update bond-damage-factor according strain tensor
  ColumnMajorMatrix n_strain(LIBMESH_DIM,1);//normal strain vector
  ColumnMajorMatrix s_strain(LIBMESH_DIM,1);// shear strain vector

  Real _directional_vector[13][LIBMESH_DIM];
  if(LIBMESH_DIM==3)
  {
    _directional_vector[ 0][0] =     1.0 ; _directional_vector[ 0][1] =     0.0 ; _directional_vector[ 0][2] =     0.0;
    _directional_vector[ 1][0] =     0.0 ; _directional_vector[ 1][1] =     1.0 ; _directional_vector[ 1][2] =     0.0;
    _directional_vector[ 2][0] =     0.0 ; _directional_vector[ 2][1] =     0.0 ; _directional_vector[ 2][2] =     1.0;
    _directional_vector[ 3][0] =     0.0 ; _directional_vector[ 3][1] = 0.70711 ; _directional_vector[ 3][2] = 0.70711;
    _directional_vector[ 4][0] =     0.0 ; _directional_vector[ 4][1] = 0.70711 ; _directional_vector[ 4][2] =-0.70711;
    _directional_vector[ 5][0] = 0.70711 ; _directional_vector[ 5][1] =     0.0 ; _directional_vector[ 5][2] = 0.70711;
    _directional_vector[ 6][0] = 0.70711 ; _directional_vector[ 6][1] =     0.0 ; _directional_vector[ 6][2] =-0.70711;
    _directional_vector[ 7][0] = 0.70711 ; _directional_vector[ 7][1] = 0.70711 ; _directional_vector[ 7][2] =     0.0;
    _directional_vector[ 8][0] = 0.70711 ; _directional_vector[ 8][1] =-0.70711 ; _directional_vector[ 8][2] =     0.0;
    _directional_vector[ 9][0] = 0.57735 ; _directional_vector[ 9][1] = 0.57735 ; _directional_vector[ 9][2] = 0.57735;
    _directional_vector[10][0] =-0.57735 ; _directional_vector[10][1] = 0.57735 ; _directional_vector[10][2] = 0.57735;
    _directional_vector[11][0] = 0.57735 ; _directional_vector[11][1] =-0.57735 ; _directional_vector[11][2] = 0.57735;
    _directional_vector[12][0] = 0.57735 ; _directional_vector[12][1] = 0.57735 ; _directional_vector[12][2] =-0.57735;
    
  }
  if(LIBMESH_DIM==2)
  {
    _directional_vector[ 0][0] =     1.0 ; _directional_vector[ 0][1] =     0.0;
    _directional_vector[ 1][0] = 0.96593 ; _directional_vector[ 1][1] = 0.25882;
    _directional_vector[ 2][0] = 0.86603 ; _directional_vector[ 2][1] = 0.50000;
    _directional_vector[ 3][0] = 0.70711 ; _directional_vector[ 3][1] = 0.70711;
    _directional_vector[ 4][0] = 0.50000 ; _directional_vector[ 4][1] = 0.86603;
    _directional_vector[ 5][0] = 0.25882 ; _directional_vector[ 5][1] = 0.96593;
    _directional_vector[ 6][0] = 0.00000 ; _directional_vector[ 6][1] = 1.00000;
    _directional_vector[ 7][0] =-0.25882 ; _directional_vector[ 7][1] = 0.96593;    
    _directional_vector[ 8][0] =-0.50000 ; _directional_vector[ 8][1] = 0.86603;
    _directional_vector[ 9][0] =-0.70711 ; _directional_vector[ 9][1] = 0.70711;
    _directional_vector[10][0] =-0.86603 ; _directional_vector[10][1] = 0.50000;
    _directional_vector[11][0] =-0.96593 ; _directional_vector[11][1] = 0.25882;
    
  }

  Real temp , temp1 , temp2;
  for(unsigned int i = 0 ; i < LIBMESH_DIM ; i++)
  {
    for(unsigned int j = 0 ; j < LIBMESH_DIM ; j++)
    {
      _total_stress(i,j) = 0.0;//initialize the stress as zero
    }
  }

  Real damage[13];
  Real damage_old[13];
  damage[ 0] = _bond_damage_factor00[qp] ; damage_old[ 0] = _bond_damage_factor00_old[qp];
  damage[ 1] = _bond_damage_factor01[qp] ; damage_old[ 1] = _bond_damage_factor01_old[qp];
  damage[ 2] = _bond_damage_factor02[qp] ; damage_old[ 2] = _bond_damage_factor02_old[qp];
  damage[ 3] = _bond_damage_factor03[qp] ; damage_old[ 3] = _bond_damage_factor03_old[qp];
  damage[ 4] = _bond_damage_factor04[qp] ; damage_old[ 4] = _bond_damage_factor04_old[qp];
  damage[ 5] = _bond_damage_factor05[qp] ; damage_old[ 5] = _bond_damage_factor05_old[qp];
  damage[ 6] = _bond_damage_factor06[qp] ; damage_old[ 6] = _bond_damage_factor06_old[qp];
  damage[ 7] = _bond_damage_factor07[qp] ; damage_old[ 7] = _bond_damage_factor07_old[qp];
  damage[ 8] = _bond_damage_factor08[qp] ; damage_old[ 8] = _bond_damage_factor08_old[qp];
  damage[ 9] = _bond_damage_factor09[qp] ; damage_old[ 9] = _bond_damage_factor09_old[qp];
  damage[10] = _bond_damage_factor10[qp] ; damage_old[10] = _bond_damage_factor10_old[qp];
  damage[11] = _bond_damage_factor11[qp] ; damage_old[11] = _bond_damage_factor11_old[qp];
  if(LIBMESH_DIM == 3 )   damage[12] = _bond_damage_factor12[qp] ; damage_old[12] = _bond_damage_factor12_old[qp];

/*
  if(LIBMESH_DIM == 3)//3 dimensional case
  {
    for (unsigned int i = 0  ; i < 13 ; i++)//loop: directions
    {
      temp = 0.0;
      for(unsigned int j = 0 ; j < LIBMESH_DIM ; j++)
      {
        for(unsigned int k = 0 ; k < LIBMESH_DIM ; k++)
        {
          temp += _total_strain(j,k) * _directional_vector[i][j] * _directional_vector[i][k];//normal strain scalar
        }
      }
      for(unsigned int j = 0 ; j < LIBMESH_DIM ; j++)   n_strain(j,0) = temp * _directional_vector[i][j];//normal strain vector
      for(unsigned int j = 0 ; j < LIBMESH_DIM ; j++)
      {
        temp = 0.0;
        for(unsigned int k = 0 ; k < LIBMESH_DIM ; k++)
        {
          temp += _total_strain(j,k) * _directional_vector[i][k];//the jth component of strain vector
        }
        s_strain(j,0) = temp - n_strain(j,0);//get shear strain vector
      }
      
      temp1 = 0.0;//normal strain scalar
      temp2 = 0.0;//shear strain scalar
      for(unsigned int j = 0 ; j < LIBMESH_DIM ; j++)
      {
        temp1 += n_strain(j,0) * _directional_vector[i][j];
        temp2 += s_strain(j,0) * s_strain(j,0);
      }
      temp2 = std::sqrt(temp2);
      temp1 = std::abs(temp1);

      //damage evolution
      temp = temp1 / _input_strain_strength + temp2 / (0.5 * (1.0 + _poissons_ratio[qp]) * _input_strain_strength);
      damage[i] = std::max(damage[i] , std::exp(-_input_damage_c * std::pow(temp,_input_damage_n)));
//      if(temp1 <= _input_strain_init) 
//      {
//        damage[i] = damage[i];
//      }
//      else if(temp1 >= _input_strain_strength)
//      {
//        damage[i] = 0.9999;
//      }
//      else
//      {
//        temp = _input_strain_init/(_input_strain_strength - _input_strain_init) * (_input_strain_strength - temp1) / temp1 ;
//        damage[i] = std::max(damage[i] , temp);
//      }

      damage[i] = std::max(damage[i] , damage_old[i]);  //temp      = std::max(damage[i] , damage_old[i]);
      //if(_q_point[qp](1) > 0.8 && _q_point[qp](1) < 0.2 )  { damage[i] = 0.0 ;} //newly added to avoid boundary damaging

      for(unsigned int j = 0 ; j < LIBMESH_DIM ; j++)
      {
        for(unsigned int k = 0 ; k < LIBMESH_DIM ; k++)
        {
          _total_stress(j,k) += (1.0 - damage[i]) * ( nstiff_tmp * n_strain(j,0) * _directional_vector[i][k] + sstiff_tmp * s_strain(j,0) * _directional_vector[i][k] );
      //    _total_stress(j,k) += (1.0 - temp) * ( nstiff_tmp * n_strain(j,0) * _directional_vector[i][k] + sstiff_tmp * s_strain(j,0) * _directional_vector[i][k] );
        }
      }
      
    }
  
  }
 
  if(LIBMESH_DIM == 2)//2 dimensional case
  {
    for (unsigned int i = 0  ; i < 12 ; i++)
    {
      //std::cout<<"The current i="<<i<<"\n";
      temp = 0.0;
      for(unsigned int j = 0 ; j < LIBMESH_DIM ; j++)
      {
        for(unsigned int k = 0 ; k < LIBMESH_DIM ; k++)
        {
          temp += _total_strain(j,k) * _directional_vector[i][j] * _directional_vector[i][k];//normal strain scalar
        }
      }
      for(unsigned int j = 0 ; j < LIBMESH_DIM ; j++)   n_strain(j,0) = temp * _directional_vector[i][j];//normal strain vector
      for(unsigned int j = 0 ; j < LIBMESH_DIM ; j++)
      {
        temp = 0.0;
        for(unsigned int k = 0 ; k < LIBMESH_DIM ; k++)
        {
          temp += _total_strain(j,k) * _directional_vector[i][k];
        }
        s_strain(j,0) = temp - n_strain(j,0);//shear strain vector
      }
      
      temp1 = 0.0;//normal strain scalar
      temp2 = 0.0;//shear strain scalar
      for(unsigned int j = 0 ; j < LIBMESH_DIM ; j++)
      {
        temp1 += n_strain(j,0) * _directional_vector[i][j];
        temp2 += s_strain(j,0) * s_strain(j,0);
      }
      temp2 = std::sqrt(temp2);
      temp1 = std::abs(temp1);

      //damage evolution
      temp = temp1 / _input_strain_strength + temp2 / (0.5 * (1.0 + _poissons_ratio[qp]) * _input_strain_strength);
      damage[i] = std::max(damage[i] , std::exp(-_input_damage_c * std::pow(temp,_input_damage_n)));
//      if(temp1 <= _input_strain_init) 
//      {
//        damage[i] = damage[i];
//      }
//      else if(temp1 >= _input_strain_strength)
//      {
//        damage[i] = 0.9999;
//      }
//      else
//      {
//        temp = _input_strain_init/(_input_strain_strength - _input_strain_init) * (_input_strain_strength - temp1) / temp1 ;
//        damage[i] = std::max(damage[i] , temp);
//      }

      damage[i] = std::max(damage[i] , damage_old[i]);  //temp      = std::max(damage[i] , damage_old[i]);
      //if(_q_point[qp](1) > 0.8 && _q_point[qp](1) < 0.2 )  { damage[i] = 0.0 ;} //newly added to avoid boundary damaging

      for(unsigned int j = 0 ; j < LIBMESH_DIM ; j++)
      {
        for(unsigned int k = 0 ; k < LIBMESH_DIM ; k++)
        {
          _total_stress(j,k) += (1.0 - damage[i]) * ( nstiff_tmp * n_strain(j,0) * _directional_vector[i][k] + sstiff_tmp * s_strain(j,0) * _directional_vector[i][k] );
      //    _total_stress(j,k) += (1.0 - temp) * ( nstiff_tmp * n_strain(j,0) * _directional_vector[i][k] + sstiff_tmp * s_strain(j,0) * _directional_vector[i][k] );
        }
      }
    }
    
  }
*/

  Real max_damage = 0.0;
  for (unsigned int i = 0  ; i < (LIBMESH_DIM > 2 ? 13 : 12) ; i++)//loop: directions
  {
    temp = 0.0;
    for(unsigned int j = 0 ; j < LIBMESH_DIM ; j++)
    {
      for(unsigned int k = 0 ; k < LIBMESH_DIM ; k++)
      {
        temp += _total_strain(j,k) * _directional_vector[i][j] * _directional_vector[i][k];//normal strain scalar
      }
    }
    for(unsigned int j = 0 ; j < LIBMESH_DIM ; j++)   n_strain(j,0) = temp * _directional_vector[i][j];//normal strain vector
    for(unsigned int j = 0 ; j < LIBMESH_DIM ; j++)
    {
      temp = 0.0;
      for(unsigned int k = 0 ; k < LIBMESH_DIM ; k++)
      {
        temp += _total_strain(j,k) * _directional_vector[i][k];//the jth component of strain vector
      }
      s_strain(j,0) = temp - n_strain(j,0);//get shear strain vector
    }
      
    temp1 = 0.0;//normal strain scalar
    temp2 = 0.0;//shear strain scalar
    for(unsigned int j = 0 ; j < LIBMESH_DIM ; j++)
    {
      temp1 += n_strain(j,0) * _directional_vector[i][j];
      temp2 += s_strain(j,0) * s_strain(j,0);
    }
    temp2 = std::sqrt(temp2);
    temp1 = std::abs(temp1);

    //damage evolution
    temp = temp1 / _input_strain_strength + temp2 / (0.5 * (1.0 + _poissons_ratio[qp]) * _input_strain_strength);
    damage[i] = std::max(damage[i] , std::exp(-_input_damage_c * std::pow(temp,_input_damage_n)));
//    if(temp1 <= _input_strain_init) 
//    {
//      damage[i] = damage[i];
//    }
//    else if(temp1 >= _input_strain_strength)
//    {
//      damage[i] = 0.9999;
//    }
//    else
//    {
//      temp = _input_strain_init/(_input_strain_strength - _input_strain_init) * (_input_strain_strength - temp1) / temp1 ;
//      damage[i] = std::max(damage[i] , temp);
//    }

    damage[i] = std::max(damage[i] , damage_old[i]);  //temp      = std::max(damage[i] , damage_old[i]);
    //if(_q_point[qp](1) > 0.8 && _q_point[qp](1) < 0.2 )  { damage[i] = 0.0 ;} //newly added to avoid boundary damaging
    max_damage = std::max(max_damage , damage[i]);
  }

  for(unsigned int i = 0  ; i < (LIBMESH_DIM > 2 ? 13 : 12) ; i++)
  {
    for(unsigned int j = 0 ; j < LIBMESH_DIM ; j++)
    {
      for(unsigned int k = 0 ; k < LIBMESH_DIM ; k++)
      {
//        _total_stress(j,k) += (1.0 - max_damage) * ( nstiff_tmp * n_strain(j,0) * _directional_vector[i][k] + sstiff_tmp * s_strain(j,0) * _directional_vector[i][k] );
        _total_stress(j,k) += (1.0 -  damage[i]) * ( nstiff_tmp * n_strain(j,0) * _directional_vector[i][k] + sstiff_tmp * s_strain(j,0) * _directional_vector[i][k] );
      }
    }
  }

  //update _bond_damage_factor
  _bond_damage_factor00[qp] = damage[ 0];
  _bond_damage_factor01[qp] = damage[ 1];
  _bond_damage_factor02[qp] = damage[ 2];
  _bond_damage_factor03[qp] = damage[ 3];
  _bond_damage_factor04[qp] = damage[ 4];
  _bond_damage_factor05[qp] = damage[ 5];
  _bond_damage_factor06[qp] = damage[ 6];
  _bond_damage_factor07[qp] = damage[ 7];
  _bond_damage_factor08[qp] = damage[ 8];
  _bond_damage_factor09[qp] = damage[ 9];
  _bond_damage_factor10[qp] = damage[10];
  _bond_damage_factor11[qp] = damage[11];
  if(LIBMESH_DIM == 3)   _bond_damage_factor12[qp] = damage[12];


  //update and return stress: 1
  _stress_normal_vector[qp](0)=_total_stress(0,0);
  _stress_normal_vector[qp](1)=_total_stress(1,1);
  if (LIBMESH_DIM == 3) _stress_normal_vector[qp](2)=_total_stress(2,2);
  _stress_shear_vector[qp](0)=_total_stress(0,1) ;
  if (LIBMESH_DIM == 3)
  {
    _stress_shear_vector[qp](1) = _total_stress(0,2);
    _stress_shear_vector[qp](2) = _total_stress(1,2) ;
  }
  //update and return stress: 2
//  _stress_normal_vector[qp](0) *= (1.-max_damage);
//  _stress_normal_vector[qp](1) *= (1.-max_damage);
//  if (LIBMESH_DIM == 3) _stress_normal_vector[qp](2) *= (1.-max_damage);
//  _stress_shear_vector[qp](0) *= (1.-max_damage);
//  if (LIBMESH_DIM == 3)
//  {
//    _stress_shear_vector[qp](1) *= (1.-max_damage);
//    _stress_shear_vector[qp](2) *= (1.-max_damage);
//  }

}
//////////////////////////////////////////////////////////////////////////





void
SolidMechanics::computeCrack_tension(const int qp)
{
  _total_strain(0,0)= _strain_normal_vector[qp](0);
  _total_strain(1,1)= _strain_normal_vector[qp](1);
  if (LIBMESH_DIM == 3) _total_strain(2,2)= _strain_normal_vector[qp](2);
  _total_strain(0,1) = _strain_shear_vector[qp](0);
  _total_strain(1,0) = _strain_shear_vector[qp](0);
  if (LIBMESH_DIM == 3)
  {
    _total_strain(0,2) = _strain_shear_vector[qp](1);
    _total_strain(2,0) = _strain_shear_vector[qp](1);
    _total_strain(1,2) = _strain_shear_vector[qp](2);
    _total_strain(2,1) = _strain_shear_vector[qp](2);    
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
    _damage_coeff[qp] = std::max(_damage_coeff_old[qp] , _damage_coeff[qp]);
    
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
//////////////////////////////////////////////////////////////////////////




void
SolidMechanics::computeCrack_Mohr_Coulomb_v1(const int qp)
{
  //copy stress to a tensor , not need to initialize, but used to do stress transforming
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


  //copy stress to a ColumnMajorMatrix , use to calculate principal stress and stress transforming
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

/*  //copy strain to a ColumnMajorMatrix
  _total_strain(0,0)= _strain_normal_vector[qp](0);
  _total_strain(1,1)= _strain_normal_vector[qp](1);
  if (LIBMESH_DIM == 3) _total_strain(2,2)= _strain_normal_vector[qp](2);
  _total_strain(0,1) = _strain_shear_vector[qp](0);
  _total_strain(1,0) = _strain_shear_vector[qp](0);
  if (LIBMESH_DIM == 3)
  {
    _total_strain(0,2) = _strain_shear_vector[qp](1);
    _total_strain(2,0) = _strain_shear_vector[qp](1);
    _total_strain(1,2) = _strain_shear_vector[qp](2);
    _total_strain(2,1) = _strain_shear_vector[qp](2);    
  }
*/

  //it is done by MOOSE automatically, not need it
 //update crack_flag before getting into calculation
 // for (unsigned int i(0); i < LIBMESH_DIM; ++i)
 // {
 //   (*_crack_flags_old)[qp](i) = (*_crack_flags)[qp](i);
 // }
  
  const int ND = LIBMESH_DIM;
  ColumnMajorMatrix e_vec(ND,ND);
  ColumnMajorMatrix norm_vect(ND,1);
  ColumnMajorMatrix force(ND,1);
  ColumnMajorMatrix principal_stress(ND,1);
  const Real tiny(1.0e-8);
  
  _total_stress1.eigen( principal_stress, e_vec );//get principal stresses and corresponding directional vector


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
  if(LIBMESH_DIM == 3)
  {
    for(int i=0 ; i < LIBMESH_DIM ; ++i )
    {
      if(i != ind_max && i != ind_min)   ind_mid = i;
    }
  }

  //rearrange principal stresses and directional vectors
  int ind_temp;
  Real temp;
  for(int i = 0 ; i < LIBMESH_DIM ; ++i)  //for directional vector are stored as column not row, transpose to row form
  {
    for(int j = 0 ; j < LIBMESH_DIM ; ++j)
    {
      temp       = e_vec(i,j);
      e_vec(i,j) = e_vec(j,i);
      e_vec(j,i) =       temp;
    }
  }
  if(ind_max > ind_min)
  {
    for(int i=0 ; i < LIBMESH_DIM ; ++i)
    {
       temp             = e_vec(ind_max,i) ;
       e_vec(ind_max,i) = e_vec(ind_min,i) ;
       e_vec(ind_min,i) = temp ; 
    }
    temp                        = principal_stress(ind_max,0) ; 
    principal_stress(ind_max,0) = principal_stress(ind_min,0) ; 
    principal_stress(ind_min,0) = temp ;
    ind_temp = ind_max;
    ind_max  = ind_min;
    ind_min = ind_temp;
  }
  if(LIBMESH_DIM ==3)
  {
    if(ind_max > ind_mid)
    {
      for (int i=0 ; i < LIBMESH_DIM ; ++i)
      {
          temp             = e_vec(ind_max,i) ;
          e_vec(ind_max,i) = e_vec(ind_mid,i) ;
	      e_vec(ind_mid,i) = temp ; 
      }
      temp                        = principal_stress(ind_max,0) ; 
      principal_stress(ind_max,0) = principal_stress(ind_mid,0) ; 
      principal_stress(ind_mid,0) = temp ;
      ind_temp = ind_max;
      ind_max  = ind_mid;
      ind_mid = ind_temp;
    }
    if(ind_mid > ind_min)
    {
      for(int i=0 ; i < LIBMESH_DIM ; ++i)
      {
        temp             = e_vec(ind_mid,i) ;
        e_vec(ind_mid,i) = e_vec(ind_min,i) ;
        e_vec(ind_min,i) = temp ; 
      }
      temp                        = principal_stress(ind_mid,0) ; 
      principal_stress(ind_mid,0) = principal_stress(ind_min,0) ; 
      principal_stress(ind_min,0) = temp ;
      ind_temp = ind_mid;
      ind_mid  = ind_min;
      ind_min = ind_temp;
    }
  }
  for(int i = 0 ; i < LIBMESH_DIM ; ++i)  //for directional vector are stored as column not row, re-transpose to column form
  {
    for(int j = 0 ; j < LIBMESH_DIM ; ++j)
    {
      temp       = e_vec(i,j);
      e_vec(i,j) = e_vec(j,i);
      e_vec(j,i) =       temp;
    }
  }
 
  //find the direction corresponding to Mohr-Coulomb
  Real temp1;
  if(LIBMESH_DIM==3)
  {
    temp = std::pow(1.0+_friction_angle*_friction_angle , 0.5) * 
         (principal_stress(0,0)-principal_stress(2,0)) / 2.0;
    temp1 = _friction_angle*_friction_angle*(principal_stress(0,0) + principal_stress(2,0))-
         2.*_cohesion*_friction_angle;
  }
  if(LIBMESH_DIM==2)
  {
    temp = std::pow(1.0+_friction_angle*_friction_angle , 0.5) * 
         (principal_stress(0,0)-principal_stress(1,0)) / 2.0;
    temp1 = _friction_angle*_friction_angle*(principal_stress(0,0) + principal_stress(1,0))-
         2.*_cohesion*_friction_angle;
  }
  temp1 /= (2.*std::pow(1.0+_friction_angle*_friction_angle , 0.5));
  Real cos2 , sin2 , cos1 , sin1;
  if(std::abs(temp) <= tiny )  //hydrostatic status
  {
    cos2 = 1.0;
    sin2 = 0.0;
    cos1 = 1.0;
    sin1 = 0.0;
  }
  else
  {
    if(std::abs(temp) >= std::abs(temp1)) 
    {
      cos2 = -temp1 / temp;
    }
    else
    {
      cos2 = -std::abs(temp1/temp) / (temp1/temp);
    }
    sin2 = std::pow(1.-cos2*cos2 , 0.5);
    sin1 = std::pow((1.-cos2)/2. , 0.5);
    if(std::abs(sin1) >= tiny)  cos1 = sin2 / 2. / sin1 ;//std::pow((1.+cos2)/2. , 0.5);
    if(std::abs(sin1) <  tiny && sin2 > 0.0)  cos1 = 1.0;
    if(std::abs(sin1) <  tiny && sin2 < 0.0)  cos1 =-1.0;
  }
  ColumnMajorMatrix rot(ND,ND) , trans(ND,ND);//rot ==> rotate principal stress to specified local coords, trans ==> rotate global coords to specified local coords
  for(int i = 0 ; i < LIBMESH_DIM ; ++i)
  {
    for(int j = 0 ; j < LIBMESH_DIM ; ++j)  rot(i,j) = 0.0;
  }
  if(LIBMESH_DIM==3)
  {
    rot(0,0) = cos1;
    rot(0,2) = sin1;
    rot(1,1) = 1.0 ;
    rot(2,0) =-sin1;
    rot(2,2) = cos1;
  }
  if(LIBMESH_DIM==2)
  {
    rot(0,0) = cos1;
    rot(0,1) = sin1;
    rot(1,0) =-sin1;
    rot(1,1) = cos1;
  }
  for(int i = 0 ; i < LIBMESH_DIM ; ++i)
  {
    for(int j = 0 ; j < LIBMESH_DIM ; ++j)
    {
      trans(i,j) = 0.0;
    //for(int k = 0 ; k < LIBMESH_DIM ; ++k)   trans(i,j) += e_vec(i,k) * rot(k,j) ;
      for(int k = 0 ; k < LIBMESH_DIM ; ++k)   trans(i,j) += rot(i,k) * e_vec(k,j) ;
    }
  }

  //rotate global stress into local coordinate
  for(int i = 0 ; i < LIBMESH_DIM ; ++i)
  {
    for(int j = 0 ; j < LIBMESH_DIM ; ++j)
    {
      _total_stress1(i,j) = 0.0;
      for(int k = 0 ; k < LIBMESH_DIM ; ++k)
      {
      //for(int m = 0 ; m < LIBMESH_DIM ; ++m)   _total_stress1(i,j) += trans(k,i) * _total_stress(k,m) * trans(m,j);
        for(int m = 0 ; m < LIBMESH_DIM ; ++m)   _total_stress1(i,j) += trans(i,k) * _total_stress(k,m) * trans(j,m);
      }
    }
  }

  //check it satisfy Mohr-Coulomb or not
  if(LIBMESH_DIM==3)
  {
    temp = (principal_stress(0,0) + principal_stress(2,0))/2.0 + 
           (principal_stress(0,0) - principal_stress(2,0))/2.0 * std::abs(cos2);//normal stress
    temp1= (principal_stress(0,0) - principal_stress(2,0))/2.0 * std::abs(sin2);// shear stress
  }
  if(LIBMESH_DIM==2)
  {
    temp = (principal_stress(0,0) + principal_stress(1,0))/2.0 + 
           (principal_stress(0,0) - principal_stress(1,0))/2.0 * std::abs(cos2);//normal stress
    temp1= (principal_stress(0,0) - principal_stress(1,0))/2.0 * std::abs(sin2);// shear stress
  }
  temp = _cohesion - _friction_angle * temp ;
  if(temp1 >= temp )  //&& _q_point[qp](1) < 0.9 && _q_point[qp](1) > 0.1) //avoid failing from BCs
  {
    (*_crack_flags)[qp](0) = tiny;
    _damage_coeff[qp] = 1.0;
  }
  else
  {
    (*_crack_flags)[qp](0) = 1.0;
  }
  (*_crack_flags)[qp](0) = std::min((*_crack_flags)[qp](0), (*_crack_flags_old)[qp](0));
  _damage_coeff[qp] = std::max(_damage_coeff_old[qp] , _damage_coeff[qp]);
  if( (*_crack_flags)[qp](0) < 0.5) //if cracked, need to release shear stress    
  {
    if(LIBMESH_DIM==3)
    {
      _total_stress1(0,2) *= tiny;
      _total_stress1(2,0) *= tiny;

//      if(_total_stress1(0,0) > 0.0 && _total_stress1(0,0) > _total_stress1(2,2))
//      {
//        _total_stress1(0,0) *= tiny;
//      }
//      if(_total_stress1(2,2) > 0.0 && _total_stress1(2,2) > _total_stress1(0,0))
//      {
//        _total_stress1(2,2) *= tiny;
//      }
      //must keep the second principal stress as second after modified stress tensor
      if(_total_stress1(1,1) > _total_stress1(0,0) && _total_stress1(1,1) > _total_stress1(2,2))
      {
        _total_stress1(1,1) = (_total_stress1(0,0) + _total_stress1(2,2)) / 2.0;
      }
      if (_total_stress1(1,1) < _total_stress1(0,0) && _total_stress1(1,1) < _total_stress1(2,2))
      {
        _total_stress1(1,1) = (_total_stress1(0,0) + _total_stress1(2,2)) / 2.0;
      }
    }
    if(LIBMESH_DIM==2)
    {
      _total_stress1(0,1) *= tiny;
      _total_stress1(1,0) *= tiny;

      if(_total_stress1(0,0) > 0.0 && _total_stress1(0,0) > _total_stress1(1,1))
      {
        _total_stress1(0,0) *= tiny;
      }
      if(_total_stress1(1,1) > 0.0 && _total_stress1(1,1) > _total_stress1(0,0))
      {
        _total_stress1(1,1) *= tiny;
      }
    }
  }

  //re-rotate the modified stress into global coordinate
  for(int i = 0 ; i < LIBMESH_DIM ; ++i)
  {
    for(int j = 0 ; j < LIBMESH_DIM ; ++j)
    {
      _total_stress(i,j) = 0.0;
      for(int k = 0 ; k < LIBMESH_DIM ; ++k)
      {
      //for(int m = 0 ; m < LIBMESH_DIM ; ++m)   _total_stress(i,j) += trans(i,k) * _total_stress1(k,m) * trans(j,m);
        for(int m = 0 ; m < LIBMESH_DIM ; ++m)   _total_stress(i,j) += trans(k,i) * _total_stress1(k,m) * trans(m,j);
      }
    }
  }    

  //retrun modified stress tensor to specified form   
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
////////////////////////////////////////////////////////////////////////




void
SolidMechanics::computeCrack_Mohr_Coulomb_v2(const int qp)
{
  //copy stress to a tensor , not need to initialize, but used to do stress transforming
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


  //copy stress to a ColumnMajorMatrix , use to calculate principal stress and stress transforming
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

/*  //copy strain to a ColumnMajorMatrix
  _total_strain(0,0)= _strain_normal_vector[qp](0);
  _total_strain(1,1)= _strain_normal_vector[qp](1);
  if (LIBMESH_DIM == 3) _total_strain(2,2)= _strain_normal_vector[qp](2);
  _total_strain(0,1) = _strain_shear_vector[qp](0);
  _total_strain(1,0) = _strain_shear_vector[qp](0);
  if (LIBMESH_DIM == 3)
  {
    _total_strain(0,2) = _strain_shear_vector[qp](1);
    _total_strain(2,0) = _strain_shear_vector[qp](1);
    _total_strain(1,2) = _strain_shear_vector[qp](2);
    _total_strain(2,1) = _strain_shear_vector[qp](2);    
  }
*/

    //it is done by MOOSE automatically, not need it
//  update crack_flag before getting into calculation
//  for (unsigned int i(0); i < LIBMESH_DIM; ++i)
//  {
//    (*_crack_flags_old)[qp](i) = (*_crack_flags)[qp](i);
//  }
  
  const int ND = LIBMESH_DIM;
  ColumnMajorMatrix e_vec(ND,ND);
//  ColumnMajorMatrix norm_vect(ND,1);
//  ColumnMajorMatrix force(ND,1);
  ColumnMajorMatrix principal_stress(ND,1);
  const Real tiny(1.0e-8);
  
  _total_stress1.eigen( principal_stress, e_vec );//get principal stresses and corresponding directional vector


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
  if(LIBMESH_DIM==3)
  {
    for(int i=0 ; i < LIBMESH_DIM ; ++i )
    {
      if(i != ind_max && i != ind_min)   ind_mid = i;
    }
  }

  //rearrange principal stresses and directional vectors
  int ind_temp;
  Real temp , temp1;
  for(int i = 0 ; i < LIBMESH_DIM ; ++i)  //for directional vector are stored as column not row, transpose to row form
  {
    for(int j = 0 ; j < LIBMESH_DIM ; ++j)
    {
      temp       = e_vec(i,j);
      e_vec(i,j) = e_vec(j,i);
     e_vec(j,i) =       temp;
    }
  }
  if(ind_max > ind_min)
  {
    for(int i=0 ; i < LIBMESH_DIM ; ++i)
    {
       temp             = e_vec(ind_max,i) ;
       e_vec(ind_max,i) = e_vec(ind_min,i) ;
       e_vec(ind_min,i) = temp ; 
    }
    temp                        = principal_stress(ind_max,0) ; 
    principal_stress(ind_max,0) = principal_stress(ind_min,0) ; 
    principal_stress(ind_min,0) = temp ;
    ind_temp = ind_max;
    ind_max  = ind_min;
    ind_min = ind_temp;
  }
  if(LIBMESH_DIM==3)
  {
    if(ind_max > ind_mid)
    {
      for (int i=0 ; i < LIBMESH_DIM ; ++i)
      {
        temp             = e_vec(ind_max,i) ;
        e_vec(ind_max,i) = e_vec(ind_mid,i) ;
	    e_vec(ind_mid,i) = temp ; 
      }
      temp                        = principal_stress(ind_max,0) ; 
      principal_stress(ind_max,0) = principal_stress(ind_mid,0) ; 
      principal_stress(ind_mid,0) = temp ;
      ind_temp = ind_max;
      ind_max  = ind_mid;
      ind_mid = ind_temp;
    }
    if(ind_mid > ind_min)
    {
      for(int i=0 ; i < LIBMESH_DIM ; ++i)
      {
        temp             = e_vec(ind_mid,i) ;
        e_vec(ind_mid,i) = e_vec(ind_min,i) ;
        e_vec(ind_min,i) = temp ; 
      }
      temp                        = principal_stress(ind_mid,0) ; 
      principal_stress(ind_mid,0) = principal_stress(ind_min,0) ; 
      principal_stress(ind_min,0) = temp ;
      ind_temp = ind_mid;
      ind_mid  = ind_min;
      ind_min = ind_temp;
    }
  }
  for(int i = 0 ; i < LIBMESH_DIM ; ++i)  //for directional vector are stored as column not row, re-transpose to column form
  {
    for(int j = 0 ; j < LIBMESH_DIM ; ++j)
    {
      temp       = e_vec(i,j);
      e_vec(i,j) = e_vec(j,i);
      e_vec(j,i) =       temp;
    }
  }


  Real nsol1 , nsol2 , ssol1 , ssol2;
  Real nstress , sstress;
  Real cos2 , sin2 , cos1 , sin1;
  ColumnMajorMatrix rot(ND,ND) , trans(ND,ND);//rot ==> rotate principal stress to specified local coords, trans ==> rotate global coords to specified local coords
  //solve the problem
  if(LIBMESH_DIM==3)
  {
    temp  = std::pow((principal_stress(0,0)+principal_stress(2,0)+2.*_cohesion*_friction_angle) , 2.0)-
	  4.*(_friction_angle*_friction_angle+1.0)*
	  (_cohesion*_cohesion+principal_stress(0,0)*principal_stress(2,0));
    temp1 = (principal_stress(0,0)+principal_stress(2,0)+2.*_cohesion*_friction_angle) / 
	  (2.*(_friction_angle*_friction_angle+1.0));
  }
  if(LIBMESH_DIM==2)
  {
    temp  = std::pow((principal_stress(0,0)+principal_stress(1,0)+2.*_cohesion*_friction_angle) , 2.0)-
	  4.*(_friction_angle*_friction_angle+1.0)*
	  (_cohesion*_cohesion+principal_stress(0,0)*principal_stress(1,0));
    temp1 = (principal_stress(0,0)+principal_stress(1,0)+2.*_cohesion*_friction_angle) / 
	  (2.*(_friction_angle*_friction_angle+1.0));
  }
  if(temp > 0.0)
  {
    nsol1 = temp1 + temp / (2.*(_friction_angle*_friction_angle+1.0));
    nsol2 = temp1 - temp / (2.*(_friction_angle*_friction_angle+1.0));
    ssol1 = _cohesion - _friction_angle * nsol1;
    ssol2 = _cohesion - _friction_angle * nsol2;
    if(std::abs(ssol1) > std::abs(ssol2)) 
    {
      sstress = ssol1;
      nstress = nsol1;
    }
    else
    {
      sstress = ssol2;
      nstress = nsol2;
    }
  }
  else if(temp == 0.0)
  {
    nstress = temp1 ; 
    sstress = _cohesion - _friction_angle * nstress;
  }
  else //(temp < 0.0)  //must within Mohr-Coulomb criteria
  {
    nstress = 0.0; //0.0 < c-k*0.0
    sstress = 0.0;
  }

  if(std::abs(nstress)>tiny || std::abs(sstress)>tiny)   // (nstress != 0.0 || sstress != 0.0) //tarch or intersect 
  {
    if(LIBMESH_DIM==3)
    {
      if((principal_stress(0,0)-principal_stress(2,0)) <= tiny) //hydrostatic
      {
        sin2 = 0.0;
        cos2 = 1.0;
        sin1 = 0.0;
        cos1 = 1.0;
      }
      else
      {
        sin2 = 2.*sstress/(principal_stress(0,0)-principal_stress(2,0));
        cos2 = 2.*(nstress-(principal_stress(0,0)+principal_stress(2,0))/2.)/(principal_stress(0,0)-principal_stress(2,0));
        temp = std::pow(sin2*sin2+cos2*cos2 , 0.5);
        sin2 /= temp;
        cos2 /= temp;

        temp  = 1. + cos2;
        temp1 = 1. - cos2;

        sin1 = std::pow(temp1/2. , 0.5) ;//always large than 0.0
        if(sin1 > tiny)  //sin2 not equal to 0.0
        {
          cos1 = sin2 / 2. / sin1 ;
        }
        else  //sin2 approach to 0.0
        {
          sin1 = 0.0;
          if(sin2 > 0.0)  cos1 = 1.0;
          if(sin2 < 0.0)  cos1 =-1.0;
        }
        temp = std::pow(sin1*sin1+cos1*cos1 , 0.5);
        cos1 /= temp;
        sin1 /= temp;
      }
    }
    if(LIBMESH_DIM==2) 
    {
      if((principal_stress(0,0)-principal_stress(1,0)) <= tiny) //hydrostatic
      {
        sin2 = 0.0;
        cos2 = 1.0;
        sin1 = 0.0;
        cos1 = 1.0;
      }
      else
      {
        sin2 = 2.*sstress/(principal_stress(0,0)-principal_stress(1,0));
        cos2 = 2.*(nstress-(principal_stress(0,0)+principal_stress(1,0))/2.)/(principal_stress(0,0)-principal_stress(1,0));
        temp = std::pow(sin2*sin2+cos2*cos2 , 0.5);
        sin2 /= temp;
        cos2 /= temp;

        temp  = 1. + cos2;
        temp1 = 1. - cos2;

        sin1 = std::pow(temp1/2. , 0.5) ;//always large than 0.0
        if(sin1 > tiny)  //sin2 not equal to 0.0
        {
          cos1 = sin2 / 2. / sin1 ;
        }
        else  //sin2 approach to 0.0
        {
          sin1 = 0.0;
          if(sin2 > 0.0)  cos1 = 1.0;
          if(sin2 < 0.0)  cos1 =-1.0;
        }
        temp = std::pow(sin1*sin1+cos1*cos1 , 0.5);
        cos1 /= temp;
        sin1 /= temp;
      }
    }
  }

  for(int i = 0 ; i < LIBMESH_DIM ; ++i)
  {
    for(int j = 0 ; j < LIBMESH_DIM ; ++j)  rot(i,j) = 0.0;
  }
  if(LIBMESH_DIM==3)
  {
    if(std::abs(nstress)>tiny || std::abs(sstress)>tiny)   // (nstress != 0.0 || sstress != 0.0)
    {
      rot(0,0) = cos1;
      rot(0,2) = sin1;
      rot(1,1) = 1.0 ;
      rot(2,0) =-sin1;
      rot(2,2) = cos1;
    }
    else
    {
      rot(0,0) = 1.0;
      rot(1,1) = 1.0;
      rot(2,2) = 1.0;
    }
  }
  if(LIBMESH_DIM==2)
  {
    if(std::abs(nstress)>tiny || std::abs(sstress)>tiny)   // (nstress != 0.0 || sstress != 0.0)
    {
      rot(0,0) = cos1;
      rot(0,1) = sin1;
      rot(1,0) =-sin1;
      rot(1,1) = cos1;
    }
    else
    {
      rot(0,0) = 1.0;
      rot(1,1) = 1.0;
    }
  }
    
  for(int i = 0 ; i < LIBMESH_DIM ; ++i)
  {
    for(int j = 0 ; j < LIBMESH_DIM ; ++j)
    {
      trans(i,j) = 0.0;
    //for(int k = 0 ; k < LIBMESH_DIM ; ++k)   trans(i,j) += e_vec(i,k) * rot(k,j) ;
      for(int k = 0 ; k < LIBMESH_DIM ; ++k)   trans(i,j) += rot(i,k) * e_vec(k,j) ;
    }
  }

  //rotate global stress into local coordinate
  for(int i = 0 ; i < LIBMESH_DIM ; ++i)
  {
    for(int j = 0 ; j < LIBMESH_DIM ; ++j)
    {
      _total_stress1(i,j) = 0.0;
      for(int k = 0 ; k < LIBMESH_DIM ; ++k)
      {
      //for(int m = 0 ; m < LIBMESH_DIM ; ++m)   _total_stress1(i,j) += trans(k,i) * _total_stress(k,m) * trans(m,j);
        for(int m = 0 ; m < LIBMESH_DIM ; ++m)   _total_stress1(i,j) += trans(i,k) * _total_stress(k,m) * trans(j,m);
      }
    }
  }

  //check it satisfy Mohr-Coulomb or not
  temp1 = std::abs(sstress);
  temp  = _cohesion - _friction_angle * nstress;
  if(temp1 >= temp )  //&& _q_point[qp](2) < 0.9 && _q_point[qp](2) > -0.9) //avoid failing from BCs
  {
    (*_crack_flags)[qp](0) = tiny;
    _damage_coeff[qp] = 1.0;
  }
  else
  {
    (*_crack_flags)[qp](0) = 1.0;
  }
  (*_crack_flags)[qp](0) = std::min((*_crack_flags)[qp](0), (*_crack_flags_old)[qp](0));
  _damage_coeff[qp] = std::max(_damage_coeff_old[qp] , _damage_coeff[qp]);
  if( (*_crack_flags)[qp](0) < 0.5) //if cracked, need to release shear stress    
  {
    if(LIBMESH_DIM==3)
    {
      _total_stress1(0,2) *= tiny;
      _total_stress1(2,0) *= tiny;

//      if(_total_stress1(0,0) > 0.0 && _total_stress1(0,0) > _total_stress1(2,2))
//      {
//        _total_stress1(0,0) *= tiny;
//      }
//      if(_total_stress1(2,2) > 0.0 && _total_stress1(2,2) > _total_stress1(0,0))
//      {
//        _total_stress1(2,2) *= tiny;
//      }
      //must keep the second principal stress as second after modified stress tensor
      if(_total_stress1(1,1) > _total_stress1(0,0) && _total_stress1(1,1) > _total_stress1(2,2))
      {
        _total_stress1(1,1) = (_total_stress1(0,0) + _total_stress1(2,2)) / 2.0;
      }
      if (_total_stress1(1,1) < _total_stress1(0,0) && _total_stress1(1,1) < _total_stress1(2,2))
      {
        _total_stress1(1,1) = (_total_stress1(0,0) + _total_stress1(2,2)) / 2.0;
      }
    }
    if(LIBMESH_DIM==2)
    {
      _total_stress1(0,1) *= tiny;
      _total_stress1(1,0) *= tiny;

//      if(_total_stress1(0,0) > 0.0 && _total_stress1(0,0) > _total_stress1(1,1))
//      {
//        _total_stress1(0,0) *= tiny;
//      }
//      if(_total_stress1(1,1) > 0.0 && _total_stress1(1,1) > _total_stress1(0,0))
//      {
//        _total_stress1(1,1) *= tiny;
//      }
    }
  }

  //re-rotate the modified stress into global coordinate
  for(int i = 0 ; i < LIBMESH_DIM ; ++i)
  {
    for(int j = 0 ; j < LIBMESH_DIM ; ++j)
    {
      _total_stress(i,j) = 0.0;
      for(int k = 0 ; k < LIBMESH_DIM ; ++k)
      {
      //for(int m = 0 ; m < LIBMESH_DIM ; ++m)   _total_stress(i,j) += trans(i,k) * _total_stress1(k,m) * trans(j,m);
        for(int m = 0 ; m < LIBMESH_DIM ; ++m)   _total_stress(i,j) += trans(k,i) * _total_stress1(k,m) * trans(m,j);
      }
    }
  } 


//  (*_crack_flags)[qp](0) = std::min((*_crack_flags)[qp](0), (*_crack_flags_old)[qp](0));
//  _damage_coeff[qp] = std::max(_damage_coeff_old[qp] , _damage_coeff[qp]);

  //retrun modified stress tensor to specified form   
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
///////////////////////////////////////////////////////////////////////

   




