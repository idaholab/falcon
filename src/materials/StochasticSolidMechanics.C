/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "StochasticSolidMechanics.h"

template<>
InputParameters validParams<StochasticSolidMechanics>()
{
  InputParameters params= validParams<StochasticPorousMedia>();
//common mechanical properties-----------------------------------------------------------------
  params.addParam<Real>("thermal_expansion",1.0e-6,"thermal expansion coefficient (1/K)");
  params.addParam<Real>("youngs_modulus",1.50e10,"in Pascal") ;
  params.addParam<Real>("poissons_ratio",0.2,"dimensionless");

  params.addParam<Real>("biot_coeff",1.0,"dimensionless");
  params.addParam<Real>("biot_modulus",2.5e10,"dimensionless");
  params.addParam<Real>("t_ref",293.15,"initial temperature");

//damage related parameters--------------------------------------------------------------------
  params.addParam<bool>("has_damage",false,"switch for turning on/off damaging mechanics");
  params.addParam<Real>("damage_coeff",0.0,"initial damage value");

  //following 4 parameters used by computeDamage- use tension failure
  params.addParam<Real>("strain_initialize_damage",0.01,"critical strain to initialize damage");
  params.addParam<Real>("strain_broken",0.04,"critical strain to complete failure");
  params.addParam<Real>("input_damage_a1",1.0,"1st parameter of Damage evolution");
  params.addParam<Real>("input_damage_a2",2.0,  "2nd parameter of Damage evolution");

  //following 4 parameters used by ComputeDamage_v2 & v3
  params.addParam<Real>("cohesion2",0.0,"Rock's Cohesion strength");
  params.addParam<Real>("friction_angle2",0.1,"Rock's internal friction angle");
  params.addParam<Real>("critical_stress",1.0e6,"tensile stress threshold");
  params.addParam<Real>("critical_strain",1.0e6,"tensile stress threshold");

//has anisotropic damage---------------------------------------------------------------------------
  params.addParam<std::string>("has_damage_method","MC_damage","switch of damage methods");
  params.addParam<Real>("input_damage_c",0.0,"the first parameter for anisotropic damage");
  params.addParam<Real>("input_damage_n",1.0,"the second parameter for anisotropic damage");
  params.addParam<Real>("input_strain_init",0.001,"the initial strain of damaging");
  params.addParam<Real>("input_strain_strength",0.0,"the third parameter for anisotropic damage");

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





StochasticSolidMechanics::StochasticSolidMechanics(const std::string & name,
                               InputParameters parameters)
  :StochasticPorousMedia(name, parameters),
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

   _has_damage(getParam<bool>("has_damage")),
   _input_damage_coeff(getParam<Real>("damage_coeff")),
   _input_strain_initialize_damage(getParam<Real>("strain_initialize_damage")),
   _input_strain_broken(getParam<Real>("strain_broken")),
   _damage_a1(getParam<Real>("input_damage_a1")),
   _damage_a2(getParam<Real>("input_damage_a2")),

   _cohesion2(getParam<Real>("cohesion2")),
   _friction_angle2(getParam<Real>("friction_angle2")),
   _critical_stress(getParam<Real>("critical_stress")),
   _critical_strain(getParam<Real>("critical_strain")),

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
   _has_damage_couple_permeability(getParam<bool>("has_damage_couple_permeability")),
   _damage_couple_permeability_coeff1(getParam<Real>("damage_couple_permeability_coeff1")),
   _damage_couple_permeability_coeff2(getParam<Real>("damage_couple_permeability_coeff2")),

   _total_strain (_dim,_dim),
   _total_stress (_dim,_dim),
   _total_stress1(_dim,_dim),

   //declare material properties
   _thermal_strain(declareProperty<Real>("thermal_strain")),
   _alpha(declareProperty<Real>("alpha")),
   _youngs_modulus(declareProperty<Real>("youngs_modulus")),
   _poissons_ratio(declareProperty<Real>("poissons_ratio")),
   _biot_coeff(declareProperty<Real>("biot_coeff")),
   _biot_modulus(declareProperty<Real>("biot_modulus")),
   _damage_coeff(declareProperty<Real>("damage_coeff")),

   _damage_indicator(declareProperty<int>("damage_indicator")),
   _damage_type_indicator(declareProperty<int>("damage_type_indicator")),

   _strain_history(declareProperty<Real>("strain_history")),
   _strain_initial_damage(declareProperty<Real>("strain_initial_damage")),
   _strain_broken_damage(declareProperty<Real>("strain_broken_damage")),

   _strain_initial_damage_old(declarePropertyOld<Real>("strain_initial_damage")),
   _strain_broken_damage_old(declarePropertyOld<Real>("strain_broken_damage")),
   _damage_coeff_old(declarePropertyOld<Real>("damage_coeff")),
   _damage_indicator_old(declarePropertyOld<int>("damage_indicator")),
   _damage_type_indicator_old(declarePropertyOld<int>("damage_type_indicator")),
   _strain_history_old(declarePropertyOld<Real>("strain_history")),

   _stress_normal_vector(declareProperty<RealVectorValue>("stress_normal_vector")),
   _stress_shear_vector (declareProperty<RealVectorValue>("stress_shear_vector")),
   _strain_normal_vector(declareProperty<RealVectorValue>("strain_normal_vector")),
   _strain_shear_vector (declareProperty<RealVectorValue>("strain_shear_vector")),
   _pstress_normal_vector(declareProperty<RealVectorValue>("pstress_normal_vector")),
   _pstrain_normal_vector(declareProperty<RealVectorValue>("pstrain_normal_vector")),

   _init_status(NULL),
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
  _init_status = &declareProperty<int>("init_status");
  if ( _has_crack && _critical_crack_strain > 0)
  {
    _crack_flags     = &declareProperty<RealVectorValue>("crack_flags");
    _crack_flags_old = &declarePropertyOld<RealVectorValue>("crack_flags");
  }

//  srand((unsigned)time(0));  //initialize random seed
}
//////////////////////////////////////////////////////////////////////////

void
StochasticSolidMechanics::computeProperties()
{
  StochasticPorousMedia::computeProperties();

// //  srand((unsigned)time(0));
//   for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
//   {
//     if  ((*_init_status)[qp] < 1)
//     {
//       Real _e_rand = randn_trig(0.0, 0.01*_input_youngs_modulus);
//       _youngs_modulus[qp] = _input_youngs_modulus + _e_rand;
//       std::cout << "_E=" << _youngs_modulus[qp] <<"\n";
//     }

//   }

for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
//    _permeability[qp] = _input_permeability;

    _youngs_modulus[qp] = _input_youngs_modulus;

    _alpha[qp]            = _input_thermal_expansion;

    _damage_coeff[qp]     = std::max(_input_damage_coeff, _damage_coeff[qp]);

    if(_has_damage_couple_permeability)
    {
      //if (_damage_coeff[qp] > 1.0e-10)
       //RKP copmment out

       //     _permeability[qp] = _init_permeability*_damage_couple_permeability_coeff2;
      //else
       // _permeability[qp] = _init_permeability;

//     _permeability[qp] = _input_permeability*(1.0 + std::pow(_damage_coeff[qp] , _damage_couple_permeability_coeff1)*_damage_couple_permeability_coeff2);
    }



    if(_has_temp)
      _thermal_strain[qp] = _input_thermal_expansion*(_temperature[qp] - _input_t_ref);
    else
      _thermal_strain[qp] = 0.0;

    _poissons_ratio[qp]   = _input_poissons_ratio;
    _biot_coeff[qp]       = _input_biot_coeff;
    _biot_modulus[qp]     = _input_biot_modulus;

    if(_dim==3)
    {
      _bond_nstiff[qp] = 3.*_youngs_modulus[qp]/(4.*3.14159265*(1.-2.*_poissons_ratio[qp]));
      _bond_sstiff[qp] = _bond_nstiff[qp]*(1.-4*_poissons_ratio[qp])/(1.+_poissons_ratio[qp]);
    }

    if(_dim==2)
    {
        // for plane-stress
        _bond_nstiff[qp] = 2.*_youngs_modulus[qp]/(3.14159265*(1.-_poissons_ratio[qp]));
        _bond_sstiff[qp] = _bond_nstiff[qp]*(1.-3.*_poissons_ratio[qp])/(1.+_poissons_ratio[qp]);
        // for plane-strain
        //  _bond_nstiff[qp] = 2.*_youngs_modulus[qp]/3.14159265/(1.+_poissons_ratio[qp])/(1.-2.*_poissons_ratio[qp]);
        //  _bond_sstiff[qp] = _bond_nstiff[qp]*(1.-4.*_poissons_ratio[qp])/2.;
    }

// first try for strain and stress vectors
    if (_has_x_disp && _has_y_disp)
    {
      _E  =  (1.0-_damage_coeff[qp])*_youngs_modulus[qp];
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

//various damage mechanics based models

   if (_has_damage && _has_damage_method == "isotropic") computeDamage(qp); //by damage mechanics theory
   if (_has_damage && _has_damage_method == "anisotropic")   computeAnisoDamage(qp);
   if (_has_damage && _has_damage_method == "MC_damage") computeDamage_v2(qp); //by damage mechanics theory
   if (_has_damage && _has_damage_method == "Pstrain_damage") computeDamage_v3(qp); //by damage mechanics theory
// first try for strain and stress vectors
    if (_has_x_disp && _has_y_disp)
    {

      _E  =  (1.0-_damage_coeff[qp])*_youngs_modulus[qp];
      _nu =  _poissons_ratio[qp];
      _c1 = _E*(1.-_nu)/(1.+_nu)/(1.-2.*_nu);
      _c2 = _nu/(1.-_nu);
      _c3 = 0.5*(1.-2.*_nu)/(1.-_nu);

      _stress_normal_vector[qp](0) = _c1*_strain_normal_vector[qp](0)+_c1*_c2*_strain_normal_vector[qp](1)+_c1*_c2*_strain_normal_vector[qp](2); //tau_xx
      _stress_normal_vector[qp](1) = _c1*_c2*_strain_normal_vector[qp](0)+_c1*_strain_normal_vector[qp](1)+_c1*_c2*_strain_normal_vector[qp](2); //tau_yy
      if (_dim == 3)
        _stress_normal_vector[qp](2) = _c1*_c2*_strain_normal_vector[qp](0)+_c1*_c2*_strain_normal_vector[qp](1)+_c1*_strain_normal_vector[qp](2); //tau_zz
 //tau_xy
      if (_dim == 3)
      {
        _stress_shear_vector[qp](1) = _c1*_c3*2.0*_strain_shear_vector[qp](1); //tau_xz
        _stress_shear_vector[qp](2) = _c1*_c3*2.0*_strain_shear_vector[qp](2); //tau_yz
      }


    }

//newly added for handling damage couple with porosity/permeability

   if(_has_damage_couple_permeability)
   {
     //  if (_damage_coeff[qp] > 1.0e-10)
//RKP comment out 6-12-12

       // _permeability[qp] = _init_permeability*_damage_couple_permeability_coeff2;
     //else
       // _permeability[qp] = _init_permeability;

//     _permeability[qp] = _input_permeability*(1.0 + std::pow(_damage_coeff[qp] , _damage_couple_permeability_coeff1)*_damage_couple_permeability_coeff2);
   }

   //smear crack model
   if (_has_crack)
   {
      if (_t_step == 1) // && _critical_crack_strain > 0)
      {
        for (unsigned int i = 0; i< _qrule->n_points(); ++i)
        {
          _damage_coeff[i]     = 0.0;
          _damage_coeff_old[i] = 0.0;
          for (unsigned int j = 0; j<_dim; ++j)
          {
            (*_crack_flags)[i](j)     = 1.0;
            (*_crack_flags_old)[i](j) = 1.0;
          }
        }
      }
      if( _has_crack_method == "tension") computeCrack_tension(qp);     //smeared crack model : tension induced crack
      if( _has_crack_method =="Mohr-Coulomb") computeCrack_Mohr_Coulomb_v2(qp); //smeared crack model : Mohr-Coulomb criteria
   }
//     if (_has_x_disp && _has_y_disp)
//     {
//       _E  =  _youngs_modulus[qp];//after updated

//       _nu =  _poissons_ratio[qp];
//       _c1 = _E*(1.-_nu)/(1.+_nu)/(1.-2.*_nu);
//       _c2 = _nu/(1.-_nu);
//       _c3 = 0.5*(1.-2.*_nu)/(1.-_nu);

//       _strain_normal_vector[qp](0) = _grad_x_disp[qp](0); //s_xx
//       _strain_normal_vector[qp](1) = _grad_y_disp[qp](1); //s_yy
//       if (_dim == 3)
//         _strain_normal_vector[qp](2) = _grad_z_disp[qp](2); //s_zz

//       _strain_shear_vector[qp](0) = 0.5*(_grad_x_disp[qp](1)+_grad_y_disp[qp](0)); // s_xy

//       if (_dim == 3)
//       {
//         _strain_shear_vector[qp](1) = 0.5*(_grad_x_disp[qp](2)+_grad_z_disp[qp](0)); // s_xz
//         _strain_shear_vector[qp](2) = 0.5*(_grad_y_disp[qp](2)+_grad_z_disp[qp](1)); // s_yz
//       }


//         _stress_normal_vector[qp](0) = _c1*_strain_normal_vector[qp](0)+_c1*_c2*_strain_normal_vector[qp](1)+_c1*_c2*_strain_normal_vector[qp](2); //tau_xx
//         _stress_normal_vector[qp](1) = _c1*_c2*_strain_normal_vector[qp](0)+_c1*_strain_normal_vector[qp](1)+_c1*_c2*_strain_normal_vector[qp](2); //tau_yy
//         if (_dim == 3)
//           _stress_normal_vector[qp](2) = _c1*_c2*_strain_normal_vector[qp](0)+_c1*_c2*_strain_normal_vector[qp](1)+_c1*_strain_normal_vector[qp](2); //tau_zz

//         _stress_shear_vector[qp](0) = _c1*_c3*2.0*_strain_shear_vector[qp](0); //tau_xy
//         if (_dim == 3)
//         {
//           _stress_shear_vector[qp](1) = _c1*_c3*2.0*_strain_shear_vector[qp](1); //tau_xz
//           _stress_shear_vector[qp](2) = _c1*_c3*2.0*_strain_shear_vector[qp](2); //tau_yz
//         }

//      }
  }

}

void
StochasticSolidMechanics::computeDamage(const int qp)
{
  Real _effective_strain=0.0 , _temp =0.0;                 //newly added
  _strain_history[qp]   = std::max(_input_strain_initialize_damage,_strain_history[qp]);
  _damage_coeff[qp]     = std::max(_input_damage_coeff, _damage_coeff[qp]);

// effect
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
//      _damage_coeff[qp]   = _damage_coeff[qp];
//      _strain_history[qp] = _strain_history[qp];
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

 // if(_q_point[qp](0) > 0.8 && _q_point[qp](0) < 0.2 )  { _damage_coeff[qp] = 0.0 ; _strain_history[qp] = 0.0; } //newly added to avoid boundary damaging

  _youngs_modulus[qp] = (1.0-_damage_coeff[qp])/(1.0-_damage_coeff_old[qp])*_youngs_modulus[qp];
}


//=================================================================================================
// developed by "Kay MIN"
// damage mechanics with Mohr-Coulomb
// damage evolution by principal strain
void
StochasticSolidMechanics::computeDamage_v2(const int qp)
{
   Real _effective_strain=0.0 , _temp =0.0;
   _strain_history[qp]   = std::max(_critical_strain, _strain_history[qp]);
   _damage_coeff[qp]     = std::max(_input_damage_coeff, _damage_coeff[qp]);

   int ind_max = 0;

 Real _sigm=0.0, _dsbar=0.0, _theta=0.0, shear_max=0.0;
 Real _dx=0.0, _dy=0.0, _dz=0.0, _xj3=0.0, _sine=0.0, _d2=0.0, _d3=0.0, _ds1=0.0, _ds2=0.0, _ds3=0.0;
 Real _s1=0.0, _s2=0.0, _s3=0.0, _s4=0.0, _s5=0.0, _s6=0.0, _temp01=0.0, _temp02=0.0, _temp03=0.0;

 _s1=_strain_normal_vector[qp](0); _s2=_strain_normal_vector[qp](1); _s3=_strain_normal_vector[qp](2);
 _s4=_strain_shear_vector[qp](0); _s5=_strain_shear_vector[qp](1); _s6=_strain_shear_vector[qp](2);

// find max.shear
    shear_max=_strain_shear_vector[qp](0);

    for(unsigned int i = 1 ; i < _dim ; ++i)
    {
      if(_strain_shear_vector[qp](i) > shear_max)
      {
        shear_max = _strain_shear_vector[qp](i);
        ind_max = i;
      }
    }

 // Strain invariants
 if (_dim==2)
 {
 	 _sigm=(_s1+_s2+_s4)/3.;
 	 _temp01=std::pow(2.,0.5);
     _dsbar=std::pow(((_s1-_s2)*(_s1-_s2)+(_s2-_s4)*(_s2-_s4)+(_s4-_s1)*(_s4-_s1)+6.*_s4*_s4)/_temp01, 0.5);
     if (_dsbar==0.)
     {
		 _theta=0.0;
	 }
	 else
	 {
	  _dx=(2.*_s1-_s2-_s4)/3.;
	  _dy=(2.*_s2-_s4-_s1)/3.;
	  _dz=(2.*_s4-_s1-_s2)/3.;
	  _xj3=std::pow(_dx*_dy*_dz-_dz*_s4,2.);
	  _sine=-(std::pow(13.5*_xj3/_dsbar,3.));
      if (_sine > 1.) _sine=1.;
      if (_sine < -1.) _sine=-1.;
      _theta=std::asin(_sine)/3.;
     }
 }
  if (_dim==3)
 {
   _sigm=(_s1+_s2+_s3)/3.; // isotropic strain volumetric strain--
	 _d2=((_s1-_s2)*(_s1-_s2)+(_s2-_s3)*(_s2-_s3)+(_s3-_s1)*(_s3-_s1))/6.+_s4*_s4+_s5*_s5+_s6*_s6;
	 _ds1=_s1-_sigm;
	 _ds2=_s2-_sigm;
	 _ds3=_s3-_sigm;
	 _temp01=std::pow(3.,0.5);
	 _d3=_ds1*_ds2*_ds3-_ds1*_s6*_s6-_ds2*_s5*_s5-_ds3*_s4*_s4+2.*_s4*_s5*_s6;
	 _temp02=std::pow(_d2,0.5);
	 _dsbar=std::sqrt(3.)*std::sqrt(_d2);
	 if (_dsbar == 0.)
	 {
		 _theta=0.;
	 }
	 else
	 {
		 _temp03=std::pow(_temp02,3.);
		 _sine=-3.*std::sqrt(3.)*_d3/(2.*std::pow(std::sqrt(_d2),3.));
         if (_sine > 1.) _sine=1.;
         if (_sine < -1.) _sine=-1.;
         _theta=std::asin(_sine)/3.;
	 }
 }


//Principal strain using invariants for 3D
	 _pstrain_normal_vector[qp](0)=(_sigm + (2.0/3.0)*_dsbar*std::sin(_theta-2.*3.14159265/3.));
	 _pstrain_normal_vector[qp](1)=(_sigm + (2.0/3.0)*_dsbar*std::sin(_theta));
	 _pstrain_normal_vector[qp](2)=(_sigm + (2.0/3.0)*_dsbar*std::sin(_theta+2.*3.14159265/3.)); //max

 // Stress invariants
 _s1=_stress_normal_vector[qp](0); _s2=_stress_normal_vector[qp](1); _s3=_stress_normal_vector[qp](2);
 _s4=_stress_shear_vector[qp](0); _s5=_stress_shear_vector[qp](1); _s6=_stress_shear_vector[qp](2);

 if (_dim==2)
 {
 	 _sigm=(_s1+_s2+_s4)/3.;
 	 _temp01=std::pow(2.,0.5);
     _dsbar=std::pow(((_s1-_s2)*(_s1-_s2)+(_s2-_s4)*(_s2-_s4)+(_s4-_s1)*(_s4-_s1)+6.*_s4*_s4)/_temp01, 0.5);
     if (_dsbar==0.)
     {
		 _theta=0.0;
	 }
	 else
	 {
	  _dx=(2.*_s1-_s2-_s4)/3.;
	  _dy=(2.*_s2-_s4-_s1)/3.;
	  _dz=(2.*_s4-_s1-_s2)/3.;
	  _xj3=std::pow(_dx*_dy*_dz-_dz*_s4,2.);
	  _sine=-(std::pow(13.5*_xj3/_dsbar,3.));
      if (_sine > 1.) _sine=1.;
      if (_sine < -1.) _sine=-1.;
      _theta=std::asin(_sine)/3.;
     }
 }
  if (_dim==3)
 {
	 _sigm=(_s1+_s2+_s3)/3.;
	 _d2=((_s1-_s2)*(_s1-_s2)+(_s2-_s3)*(_s2-_s3)+(_s3-_s1)*(_s3-_s1))/6.+_s4*_s4+_s5*_s5+_s6*_s6;
	 _ds1=_s1-_sigm;
	 _ds2=_s2-_sigm;
	 _ds3=_s3-_sigm;
	 _temp01=std::pow(3.,0.5);
	 _d3=_ds1*_ds2*_ds3-_ds1*_s6*_s6-_ds2*_s5*_s5-_ds3*_s4*_s4+2.*_s4*_s5*_s6;
	 _temp02=std::pow(_d2,0.5);
	 _dsbar=std::sqrt(3.)*std::sqrt(_d2);
	 if (_dsbar == 0.)
	 {
		 _theta=0.;
	 }
	 else
	 {
		 _temp03=std::pow(_temp02,3.);
		 _sine=-3.*std::sqrt(3.)*_d3/(2.*std::pow(std::sqrt(_d2),3.));
         if (_sine > 1.) _sine=1.;
         if (_sine < -1.) _sine=-1.;
         _theta=std::asin(_sine)/3.;
	 }
 }

//Principal stress using invariants for 3D
	 _pstress_normal_vector[qp](0)=(_sigm + (2.0/3.0)*_dsbar*std::sin(_theta-2.*3.141592653589793/3.));
	 _pstress_normal_vector[qp](1)=(_sigm + (2.0/3.0)*_dsbar*std::sin(_theta));
	 _pstress_normal_vector[qp](2)=(_sigm + (2.0/3.0)*_dsbar*std::sin(_theta+2.*3.141592653589793/3.)); //max

 //Mohr-Coulomb Failure criterion
 Real _phir=0.0, _snph=0.0, _csph=0.0, _csth=0.0, _snth=0.0, _f=0.0;

_temp01=std::atan(1.);
_temp02=std::pow(3.,0.5);
_phir=_friction_angle2*4.*std::atan(1.)/180.;
_snph=std::sin(_phir);
_csph=std::cos(_phir);
_csth=std::cos(_theta);
_snth=std::sin(_theta);
_f=_snph*_sigm+_dsbar*(_csth/std::sqrt(3.)-_snth*_snph/3.)-_cohesion2*_csph;

//Damage evolution based on Mohr-Coulomb
if (_f > 0. && _pstress_normal_vector[qp](2) > _critical_stress)  //tensile failure
//if (_pstrain_normal_vector[qp](2) > _critical_strain)
{
  _f=10.;

  //damage initiation indicator for tensile
  if ( _damage_indicator_old[qp] != 1)
  {
    std::cout<<"damage initiate"<<_f<<"\n";
    _damage_indicator[qp] = 1; //1:damage initiation
    _damage_type_indicator[qp] = 1; // 1:tensile, 2:shear
    _effective_strain = _pstrain_normal_vector[qp](2); //max
    _strain_initial_damage[qp] = _effective_strain;
    _strain_broken_damage[qp] = _effective_strain*10.;
  }
  else if (_damage_indicator_old[qp] == 1)
  {
    _strain_initial_damage[qp] = _strain_initial_damage_old[qp];
    _strain_broken_damage[qp] = _strain_broken_damage_old[qp];
    _damage_indicator[qp]=1;
    _damage_type_indicator[qp]=1;
  }
}

if (_f > 0. && shear_max > _critical_strain)          //shear failure
{
	_f=20.;
}
//damage initiation indicator for shear
//else if (_f > 0. && _damage_indicator_old[qp] != 1)
else if (_f==20. && _damage_indicator_old[qp] != 1)
{
  if ( _damage_indicator_old[qp] != 1)
  {
    std::cout<<"damage initiate"<<_f<<"\n";
    _damage_indicator[qp] = 1; //1:damage initiation
    _damage_type_indicator[qp] = 2; // 1:tensile, 2:shear
    _effective_strain = shear_max; //max
    _strain_initial_damage[qp] = _effective_strain;
    _strain_broken_damage[qp] = _effective_strain*5.;
  }
  else if (_damage_indicator_old[qp] == 1)
  {
    _strain_initial_damage[qp] = _strain_initial_damage_old[qp];
    _strain_broken_damage[qp] = _strain_broken_damage_old[qp];
    _damage_indicator[qp]=1;
    _damage_type_indicator[qp]=2;
  }
}


//tensile failure
if (_damage_indicator[qp] == 1 && _damage_type_indicator[qp] == 1)
{
	_effective_strain = _pstrain_normal_vector[qp](2); //max

  if(_strain_history[qp] < _strain_broken_damage[qp]) //not fully failed
  {
    if(_effective_strain <= _strain_history[qp])//no further damaging
    {
//      _damage_coeff[qp]   = _damage_coeff[qp];
//      _strain_history[qp] = _strain_history[qp];
    }
    else //damaging continues
    {
      if(_effective_strain >= _strain_broken_damage[qp])//fully failed: s > sc
      {
        _damage_coeff[qp]   = 0.9999999;
        _strain_history[qp] = _effective_strain;
      }
      else//continuously damaging s-{s0,sc}
      {
        _temp = (_effective_strain - _strain_initial_damage[qp])/(_strain_broken_damage[qp] - _strain_initial_damage[qp]);
        _temp = _damage_a1 * std::pow(_temp , _damage_a2) + _input_damage_coeff;
        if(_temp >= 0.99)
        {
          std::cout<<"The parameter of damage evolution are too large,please change them"<<"\n";
          _temp = 0.9999999;
        }
        _damage_coeff[qp] = std::max(_temp , _damage_coeff[qp]);
        _strain_history[qp] = _effective_strain;
      }
    }
//   else if (_effective_strain <= _strain_initial_damage[qp])//no damaging yet
//   {
//	    _damage_coeff[qp]   = 0.0;
//        _strain_history[qp] = _effective_strain;
//   }
  }
  else //alreadry completely failed
  {
    _damage_coeff[qp] = 0.9999999;
    _strain_history[qp] =  std::max(_strain_history[qp] , _effective_strain);
  }
}

//shear failure
if (_damage_indicator[qp] == 1 && _damage_type_indicator[qp] == 2)
{
	_effective_strain = _pstrain_normal_vector[qp](2); //max

  if(_strain_history[qp] < _strain_broken_damage[qp]) //not fully failed
  {
    if(_effective_strain <= _strain_history[qp])//no further damaging
    {
//      _damage_coeff[qp]   = _damage_coeff[qp];
//      _strain_history[qp] = _strain_history[qp];
    }
    else //damaging continues
    {
      if(_effective_strain >= _strain_broken_damage[qp])//fully failed: s > sc
      {
        _damage_coeff[qp]   = 0.9999999;
        _strain_history[qp] = _effective_strain;
      }
      else//continuously damaging s-{s0,sc}
      {
        _temp = (_effective_strain - _strain_initial_damage[qp])/(_strain_broken_damage[qp] - _strain_initial_damage[qp]);
        _temp = _damage_a1 * std::pow(_temp , _damage_a2) + _input_damage_coeff;
        if(_temp >= 0.8)
        {
          std::cout<<"The parameter of damage evolution are too large,please change them"<<"\n";
          _temp = 0.9999999;
        }
        _damage_coeff[qp] = std::max(_temp , _damage_coeff[qp]);
        _strain_history[qp] = _effective_strain;
      }
    }
//   else if (_effective_strain <= _strain_initial_damage[qp])//no damaging yet
//   {
//	    _damage_coeff[qp]   = 0.0;
//        _strain_history[qp] = _effective_strain;
//   }
  }
  else //alreadry completely failed
  {
    _damage_coeff[qp] = 0.9999999;
    _strain_history[qp] =  std::max(_strain_history[qp] , _effective_strain);
  }
}

if(_f<= 0.0 && _damage_indicator[qp] != 1)
{
  _damage_coeff[qp] = 0.0;
  _strain_history[qp] =  _strain_history_old[qp];
}

  //newly added to comply MOOSE procedure
  _damage_coeff[qp]   = std::max(_damage_coeff[qp] , _damage_coeff_old[qp]);
  _strain_history[qp] = std::max(_strain_history[qp] , _strain_history_old[qp]);
//   _youngs_modulus[qp] = (1.0-_damage_coeff[qp])*_input_youngs_modulus;
//  if(_damage_coeff[qp]>0.8)  { _youngs_modulus[qp] = (1.0-_damage_coeff[qp])*_input_youngs_modulus/100.; }
//  if(_q_point[qp](1) > 1.7 || _q_point[qp](1) < 0.3 )
}
//=================================================================================================

//=================================================================================================
// developed by "Kay MIN"
// damage mechanics with Principal strain
// damage evolution by principal strain


void
StochasticSolidMechanics::computeDamage_v3(const int qp)
{
   Real _effective_strain=0.0 , _temp =0.0;
   _strain_history[qp]   = std::max(_critical_strain,_strain_history[qp]);
   _damage_coeff[qp]     = std::max(_input_damage_coeff, _damage_coeff[qp]);


   int ind_max = 0;
   int ind_min = 0;
   int ind_mid = 0;

 // Strain invariants
 Real shear_max=0.0;

// find max.shear
    shear_max=_strain_shear_vector[qp](0);

    for(unsigned int i = 1 ; i < _dim ; ++i)
    {
      if(_strain_shear_vector[qp](i) > shear_max)
      {
        shear_max = _strain_shear_vector[qp](i);
        ind_max = i;
      }
    }

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

  const int ND = _dim;
  ColumnMajorMatrix e_vec(ND,ND);
  ColumnMajorMatrix principal_strain(ND,1);
  ColumnMajorMatrix principal_stress(ND,1);

  _total_strain.eigen( principal_strain, e_vec );//get principal
  _total_stress1.eigen( principal_stress, e_vec );//get principal


Real s_max,s_min,ss_max,ss_min;

// find out minimum and maximum principal stress

    ss_max = principal_stress(0,0);
    ss_min = principal_stress(0,0);


  for(unsigned int i = 1 ; i < _dim ; ++i)
  {
    if(principal_stress(i,0) > ss_max)
    {
      ss_max = principal_stress(i,0);
      ind_max = i;
    }
    if(principal_stress(i,0) < ss_min)
    {
      ss_min = principal_stress(i,0);
      ind_min = i;
    }
  }
  if(_dim == 3)
  {
    for(unsigned int i=0 ; i < _dim ; ++i )
    {
      if(i != ind_max && i != ind_min)   ind_mid = i;
    }
  }

	 _pstress_normal_vector[qp](0)=ss_min;
	 _pstress_normal_vector[qp](1)=principal_stress(ind_mid,0);
	 _pstress_normal_vector[qp](2)=ss_max;

 s_max = principal_strain(0,0);
 s_min = principal_strain(0,0);

// find out minimum and maximum principal strain
  for(unsigned int i = 1 ; i < _dim ; ++i)
  {
    if(principal_strain(i,0) > s_max)
    {
      s_max = principal_strain(i,0);
      ind_max = i;
    }
    if(principal_strain(i,0) < s_min)
    {
      s_min = principal_strain(i,0);
      ind_min = i;
    }
  }
  if(_dim == 3)
  {
    for(unsigned int i=0 ; i < _dim ; ++i )
    {
      if(i != ind_max && i != ind_min)   ind_mid = i;
    }
  }

	 _pstrain_normal_vector[qp](0)=s_min;
	 _pstrain_normal_vector[qp](1)=principal_strain(ind_mid,0);
	 _pstrain_normal_vector[qp](2)=s_max;

int _f;

if ( s_max >= _critical_strain)
{
	_f=10;

	//damage initiation indicator for tensile
	if ( _damage_indicator_old[qp] != 1)
	{
		std::cout<<"damage initiate"<<_f<<"\n";
		_damage_indicator[qp] = 1; //1:damage initiation
		_damage_type_indicator[qp] = 1; // 1:tensile, 2:shear
		_effective_strain = s_max; //max
		_strain_initial_damage[qp] = _effective_strain;
		_strain_broken_damage[qp] = _effective_strain*5.;
	}
   if (_damage_indicator_old[qp] == 1)
   {
	   	_strain_initial_damage[qp] = _strain_initial_damage_old[qp];
		_strain_broken_damage[qp] = _strain_broken_damage_old[qp];
		_damage_indicator[qp]=1;
	    _damage_type_indicator[qp]=1;
   }
}

//tensile failure
if (_damage_indicator[qp] == 1 && _damage_type_indicator[qp] == 1)
{
	_effective_strain = s_max; //max


  if(_strain_history[qp] < _strain_broken_damage[qp]) //not fully failed
  {
    if(_effective_strain <= _strain_history[qp])//no further damaging
    {
//      _damage_coeff[qp]   = _damage_coeff[qp];
//      _strain_history[qp] = _strain_history[qp];
    }
    else //damaging continues
    {
      if(_effective_strain >= _strain_broken_damage[qp])//fully failed: s > sc
      {
        _damage_coeff[qp]   = 0.9999999;
        _strain_history[qp] = _effective_strain;
      }
      else//continuously damaging s-{s0,sc}
      {

        _temp = (_effective_strain - _strain_initial_damage[qp])/(_strain_broken_damage[qp] - _strain_initial_damage[qp]);
        _temp = _damage_a1 * std::pow(_temp , _damage_a2) + _input_damage_coeff;
        if(_temp >= 0.99)
        {
          std::cout<<"The parameter of damage evolution are too large,please change them"<<"\n";
          _temp = 0.9999999;
        }
        _damage_coeff[qp] = std::max(_temp , _damage_coeff[qp]);
        _strain_history[qp] = _effective_strain;
        std::cout<<"damage propagated"<<_damage_coeff[qp]<<"\n";
      }
    }
//   else if (_effective_strain <= _strain_initial_damage[qp])//no damaging yet
//   {
//	    _damage_coeff[qp]   = 0.0;
//        _strain_history[qp] = _effective_strain;
//   }
  }
  else //alreadry completely failed
  {
    _damage_coeff[qp] = 0.9999999;
    _strain_history[qp] =  std::max(_strain_history[qp] , _effective_strain);
  }
}

  //newly added to comply MOOSE procedure
  _damage_coeff[qp]   = std::max(_damage_coeff[qp] , _damage_coeff_old[qp]);
  _strain_history[qp] = std::max(_strain_history[qp] , _strain_history_old[qp]);

   _youngs_modulus[qp] = (1.0-_damage_coeff[qp])*_input_youngs_modulus;

  if(_damage_coeff[qp]>0.9)  { _youngs_modulus[qp] = (1.0-_damage_coeff[qp])*_input_youngs_modulus/100.; }

  if(_q_point[qp](1) > 1.7  || _q_point[qp](1) < 0.3)
  {
		_damage_coeff[qp] = 0.0 ;
		_strain_history[qp] = 0.0;
		_youngs_modulus[qp] = _input_youngs_modulus;
  } //newly added to avoid boundary damaging


}

//=================================================================================================

void
StochasticSolidMechanics::computeAnisoDamage(const int qp) //just calculate damage evolution according to strain not stress tensor
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

}
//////////////////////////////////////////////////////////////////////////


void
StochasticSolidMechanics::computeCrack_tension(const int qp)
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
//  for (unsigned int i(0); i < _dim; ++i)
//  {
//    (*_crack_flags_old)[qp](i) = (*_crack_flags)[qp](i);
//  }

  const int ND = _dim;
  ColumnMajorMatrix e_vec(ND,ND);
  ColumnMajorMatrix principal_strain(ND,1);
  ColumnMajorMatrix principal_stress(ND,1);
  _total_strain.eigen( principal_strain, e_vec );

  const Real tiny(1.0e-8);

  for (unsigned int i(0); i < _dim; ++i)
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
  for (unsigned int i(0); i < _dim; ++i)
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

  for (unsigned int i(0); i < _dim; ++i)
  {
    if (crack_flags(i) < 0.5)
    {
      principal_stress(i,0) = tiny;
    }
  }

ColumnMajorMatrix trans(_dim,_dim);

  for (unsigned int j(0); j < _dim; ++j)
  {
    for (unsigned int i(0); i < _dim; ++i)
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
//////////////////////////////////////////////////////////////////////////





void
StochasticSolidMechanics::rotateSymmetricTensor( const ColumnMajorMatrix & R,
                                      const RealTensorValue & T,
                                      RealTensorValue & result )
{

  //     R           T         Rt
  //  00 01 02   00 01 02   00 10 20
  //  10 11 12 * 10 11 12 * 01 11 21
  //  20 21 22   20 21 22   02 12 22
  //
  if ( _dim == 3)
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
  else if ( _dim == 2)
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
StochasticSolidMechanics::computeCrack_Mohr_Coulomb_v1(const int qp)
{
  //copy stress to a tensor , not need to initialize, but used to do stress transforming
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


  //copy stress to a ColumnMajorMatrix , use to calculate principal stress and stress transforming
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

/*  //copy strain to a ColumnMajorMatrix
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
*/

  //it is done by MOOSE automatically, not need it
 //update crack_flag before getting into calculation
 // for (unsigned int i(0); i < _dim; ++i)
 // {
 //   (*_crack_flags_old)[qp](i) = (*_crack_flags)[qp](i);
 // }

  const int ND = _dim;
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
  for(unsigned int i = 1 ; i < _dim ; ++i)
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
  if(_dim == 3)
  {
    for(unsigned int i=0 ; i < _dim ; ++i )
    {
      if(i != ind_max && i != ind_min)   ind_mid = i;
    }
  }

  //rearrange principal stresses and directional vectors
  int ind_temp;
  Real temp;
  for(unsigned int i = 0 ; i < _dim ; ++i)  //for directional vector are stored as column not row, transpose to row form
  {
    for(unsigned int j = 0 ; j < _dim ; ++j)
    {
      temp       = e_vec(i,j);
      e_vec(i,j) = e_vec(j,i);
      e_vec(j,i) =       temp;
    }
  }
  if(ind_max > ind_min)
  {
    for(int i=0 ; i < _dim ; ++i)
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
  if(_dim ==3)
  {
    if(ind_max > ind_mid)
    {
      for (int i=0 ; i < _dim ; ++i)
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
      for(int i=0 ; i < _dim ; ++i)
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
  for(int i = 0 ; i < _dim ; ++i)  //for directional vector are stored as column not row, re-transpose to column form
  {
    for(int j = 0 ; j < _dim ; ++j)
    {
      temp       = e_vec(i,j);
      e_vec(i,j) = e_vec(j,i);
      e_vec(j,i) =       temp;
    }
  }

  //find the direction corresponding to Mohr-Coulomb
  Real temp1;
  if(_dim==3)
  {
    temp = std::pow(1.0+_friction_angle*_friction_angle , 0.5) *
         (principal_stress(0,0)-principal_stress(2,0)) / 2.0;
    temp1 = _friction_angle*_friction_angle*(principal_stress(0,0) + principal_stress(2,0))-
         2.*_cohesion*_friction_angle;
  }
  if(_dim==2)
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
  for(int i = 0 ; i < _dim ; ++i)
  {
    for(int j = 0 ; j < _dim ; ++j)  rot(i,j) = 0.0;
  }
  if(_dim==3)
  {
    rot(0,0) = cos1;
    rot(0,2) = sin1;
    rot(1,1) = 1.0 ;
    rot(2,0) =-sin1;
    rot(2,2) = cos1;
  }
  if(_dim==2)
  {
    rot(0,0) = cos1;
    rot(0,1) = sin1;
    rot(1,0) =-sin1;
    rot(1,1) = cos1;
  }
  for(int i = 0 ; i < _dim ; ++i)
  {
    for(int j = 0 ; j < _dim ; ++j)
    {
      trans(i,j) = 0.0;
    //for(int k = 0 ; k < _dim ; ++k)   trans(i,j) += e_vec(i,k) * rot(k,j) ;
      for(int k = 0 ; k < _dim ; ++k)   trans(i,j) += rot(i,k) * e_vec(k,j) ;
    }
  }

  //rotate global stress into local coordinate
  for(int i = 0 ; i < _dim ; ++i)
  {
    for(int j = 0 ; j < _dim ; ++j)
    {
      _total_stress1(i,j) = 0.0;
      for(int k = 0 ; k < _dim ; ++k)
      {
      //for(int m = 0 ; m < _dim ; ++m)   _total_stress1(i,j) += trans(k,i) * _total_stress(k,m) * trans(m,j);
        for(int m = 0 ; m < _dim ; ++m)   _total_stress1(i,j) += trans(i,k) * _total_stress(k,m) * trans(j,m);
      }
    }
  }

  //check it satisfy Mohr-Coulomb or not
  if(_dim==3)
  {
    temp = (principal_stress(0,0) + principal_stress(2,0))/2.0 +
           (principal_stress(0,0) - principal_stress(2,0))/2.0 * std::abs(cos2);//normal stress
    temp1= (principal_stress(0,0) - principal_stress(2,0))/2.0 * std::abs(sin2);// shear stress
  }
  if(_dim==2)
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
    if(_dim==3)
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
    if(_dim==2)
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
  for(int i = 0 ; i < _dim ; ++i)
  {
    for(int j = 0 ; j < _dim ; ++j)
    {
      _total_stress(i,j) = 0.0;
      for(int k = 0 ; k < _dim ; ++k)
      {
      //for(int m = 0 ; m < _dim ; ++m)   _total_stress(i,j) += trans(i,k) * _total_stress1(k,m) * trans(j,m);
        for(int m = 0 ; m < _dim ; ++m)   _total_stress(i,j) += trans(k,i) * _total_stress1(k,m) * trans(m,j);
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




void
StochasticSolidMechanics::computeCrack_Mohr_Coulomb_v2(const int qp)
{
  //copy stress to a tensor , not need to initialize, but used to do stress transforming
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


  //copy stress to a ColumnMajorMatrix , use to calculate principal stress and stress transforming
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

/*  //copy strain to a ColumnMajorMatrix
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
*/

    //it is done by MOOSE automatically, not need it
//  update crack_flag before getting into calculation
//  for (unsigned int i(0); i < _dim; ++i)
//  {
//    (*_crack_flags_old)[qp](i) = (*_crack_flags)[qp](i);
//  }

  const int ND = _dim;
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
  for(int i = 1 ; i < _dim ; ++i)
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
  if(_dim==3)
  {
    for(int i=0 ; i < _dim ; ++i )
    {
      if(i != ind_max && i != ind_min)   ind_mid = i;
    }
  }

  //rearrange principal stresses and directional vectors
  int ind_temp;
  Real temp , temp1;
  for(int i = 0 ; i < _dim ; ++i)  //for directional vector are stored as column not row, transpose to row form
  {
    for(int j = 0 ; j < _dim ; ++j)
    {
      temp       = e_vec(i,j);
      e_vec(i,j) = e_vec(j,i);
     e_vec(j,i) =       temp;
    }
  }
  if(ind_max > ind_min)
  {
    for(int i=0 ; i < _dim ; ++i)
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
  if(_dim==3)
  {
    if(ind_max > ind_mid)
    {
      for (int i=0 ; i < _dim ; ++i)
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
      for(int i=0 ; i < _dim ; ++i)
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
  for(int i = 0 ; i < _dim ; ++i)  //for directional vector are stored as column not row, re-transpose to column form
  {
    for(int j = 0 ; j < _dim ; ++j)
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
  if(_dim==3)
  {
    temp  = std::pow((principal_stress(0,0)+principal_stress(2,0)+2.*_cohesion*_friction_angle) , 2.0)-
	  4.*(_friction_angle*_friction_angle+1.0)*
	  (_cohesion*_cohesion+principal_stress(0,0)*principal_stress(2,0));
    temp1 = (principal_stress(0,0)+principal_stress(2,0)+2.*_cohesion*_friction_angle) /
	  (2.*(_friction_angle*_friction_angle+1.0));
  }
  if(_dim==2)
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
    if(_dim==3)
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
    if(_dim==2)
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

  for(int i = 0 ; i < _dim ; ++i)
  {
    for(int j = 0 ; j < _dim ; ++j)  rot(i,j) = 0.0;
  }
  if(_dim==3)
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
  if(_dim==2)
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

  for(int i = 0 ; i < _dim ; ++i)
  {
    for(int j = 0 ; j < _dim ; ++j)
    {
      trans(i,j) = 0.0;
    //for(int k = 0 ; k < _dim ; ++k)   trans(i,j) += e_vec(i,k) * rot(k,j) ;
      for(int k = 0 ; k < _dim ; ++k)   trans(i,j) += rot(i,k) * e_vec(k,j) ;
    }
  }

  //rotate global stress into local coordinate
  for(int i = 0 ; i < _dim ; ++i)
  {
    for(int j = 0 ; j < _dim ; ++j)
    {
      _total_stress1(i,j) = 0.0;
      for(int k = 0 ; k < _dim ; ++k)
      {
      //for(int m = 0 ; m < _dim ; ++m)   _total_stress1(i,j) += trans(k,i) * _total_stress(k,m) * trans(m,j);
        for(int m = 0 ; m < _dim ; ++m)   _total_stress1(i,j) += trans(i,k) * _total_stress(k,m) * trans(j,m);
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
    if(_dim==3)
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
    if(_dim==2)
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
  for(int i = 0 ; i < _dim ; ++i)
  {
    for(int j = 0 ; j < _dim ; ++j)
    {
      _total_stress(i,j) = 0.0;
      for(int k = 0 ; k < _dim ; ++k)
      {
      //for(int m = 0 ; m < _dim ; ++m)   _total_stress(i,j) += trans(i,k) * _total_stress1(k,m) * trans(j,m);
        for(int m = 0 ; m < _dim ; ++m)   _total_stress(i,j) += trans(k,i) * _total_stress1(k,m) * trans(m,j);
      }
    }
  }


//  (*_crack_flags)[qp](0) = std::min((*_crack_flags)[qp](0), (*_crack_flags_old)[qp](0));
//  _damage_coeff[qp] = std::max(_damage_coeff_old[qp] , _damage_coeff[qp]);

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
///////////////////////////////////////////////////////////////////////






