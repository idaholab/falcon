#include "ThermalPoroElastic.h"

template<>
InputParameters validParams<ThermalPoroElastic>()
{
  InputParameters params;
  
  params.set<Real>("permeability")         =  1.0e-12; //intrinsic permeability, "k", in (m^2)
  params.set<Real>("porosity")             =  0.2;    //dimensionless but variable
  params.set<Real>("rho_r")                =  2.50e3;  //rock density, in  (kg/m^3)
  params.set<Real>("rock_specific_heat")   =  0.92e3;  //units of (J/(kg K))
  params.set<Real>("thermal_conductivity") =  2.4;     //thermal thermal_conductivity, in (W/mK)
  params.set<Real>("thermal_expansion")    =  1.0e-6;  //thermal expansion coefficient (1/K)
  params.set<Real>("youngs_modulus")       = 15.0e09;  //(in Pascal) 
  params.set<Real>("poissons_ratio")       =  0.2;
  params.set<Real>("biot_coeff")           =  1.0;
  params.set<Real>("t_ref")                = 293.15;     //in K,equivalent to 20 C

  params.set<Real>("rho_w")                = 1000.0;   //water density, variable, in (kg/m^3)
  params.set<Real>("mu_w")                 = 0.001;    //water dynamic viscosity, variable, in (Pa s)
  params.set<Real>("c_f")                  = 4.6e-10;  //fluid compressibility (1/Pa)
  params.set<Real>("water_specific_heat")  = 4.186e3;   //units of (J/(kg K))

  params.set<bool>("temp_dependent_density")  = true;
  params.set<bool>("has_solid_mechanics") = true;

  params.set<Real>("gravity") = 9.80665; //gravity acceleration, in (m/s^2)
  params.set<Real>("gx") = 0.0;           //x component of the gravity pressure vector
  params.set<Real>("gy") = 0.0;           //y component of the gravity pressure vector
  params.set<Real>("gz") = 1.0;           //z component of the gravity pressure vector

  return params;
}

ThermalPoroElastic::ThermalPoroElastic(std::string name,
           InputParameters parameters,
           unsigned int block_id,
           std::vector<std::string> coupled_to,
           std::vector<std::string> coupled_as)
    :Material(name,parameters,block_id,coupled_to,coupled_as),
     _has_pressure(isCoupled("pressure")),
     _grad_p  (_has_pressure ? coupledGrad("pressure") : _grad_zero),
     _pressure(_has_pressure ? coupledVal("pressure")  : _zero),

     _has_temp(isCoupled("temperature")),
     _grad_temp  (_has_temp ? coupledGrad("temperature") : _grad_zero),
     _temperature(_has_temp ? coupledVal("temperature")  : _zero),
      _has_x_disp(isCoupled("x_disp")),
     _grad_x_disp(_has_x_disp ? coupledGrad("x_disp") : _grad_zero),
      _has_y_disp(isCoupled("y_disp")),
     _grad_y_disp(_has_y_disp ? coupledGrad("y_disp") : _grad_zero),
      _has_z_disp(isCoupled("z_disp")),
     _grad_z_disp(_has_z_disp ? coupledGrad("z_disp") : _grad_zero),
     
     _has_solid_mechanics(parameters.get<bool>("has_solid_mechanics")),
     _has_variable_density(parameters.get<bool>("temp_dependent_density")),
     _input_permeability(parameters.get<Real>("permeability")),
     _input_porosity(parameters.get<Real>("porosity")),
     _input_rho_r(parameters.get<Real>("rho_r")),
     _input_rock_specific_heat(parameters.get<Real>("rock_specific_heat")),
     _input_thermal_conductivity(parameters.get<Real>("thermal_conductivity")),
     _input_thermal_expansion(parameters.get<Real>("thermal_expansion")),
     _input_youngs_modulus(parameters.get<Real>("youngs_modulus")),
     _input_poissons_ratio(parameters.get<Real>("poissons_ratio")),
     _input_biot_coeff(parameters.get<Real>("biot_coeff")),     
     _input_t_ref(parameters.get<Real>("t_ref")),

     _input_rho_w(parameters.get<Real>("rho_w")),
     _input_mu_w(parameters.get<Real>("mu_w")),
     _input_c_f(parameters.get<Real>("c_f")),
     _input_water_specific_heat(parameters.get<Real>("water_specific_heat")),     
     
     _input_gravity(parameters.get<Real>("gravity")),
     _gx(parameters.get<Real>("gx")),
     _gy(parameters.get<Real>("gy")),
     _gz(parameters.get<Real>("gz")),

   //delcare material properties
     _permeability(declareRealProperty("permeability")),
     _porosity(declareRealProperty("porosity")),
     _rho_r(declareRealProperty("rho_r")),
     _rock_specific_heat(declareRealProperty("rock_specific_heat")),
     _thermal_conductivity(declareRealProperty("thermal_conductivity")),     
     _thermal_strain(declareRealProperty("thermal_strain")),
     _alpha(declareRealProperty("alpha")),
     _youngs_modulus(declareRealProperty("youngs_modulus")),
     _poissons_ratio(declareRealProperty("poissons_ratio")),
     _biot_coeff(declareRealProperty("biot_coeff")),     
     
     _rho_w(declareRealProperty("rho_w")),
     _mu_w(declareRealProperty("mu_w")),
     _c_f(declareRealProperty("c_f")),
     _water_specific_heat(declareRealProperty("water_specific_heat")),
     
     _darcy_params(declareRealProperty("darcy_params")),
     _darcy_flux(declareGradientProperty("darcy_flux")),
     _pore_velocity(declareGradientProperty("pore_velocity")),

     _gravity(declareRealProperty("gravity")),
     _gravity_vector(declareRealVectorValueProperty("gravity_vector")),
     _strain_normal_vector(declareRealVectorValueProperty("strain_normal_vector")),
     _strain_shear_vector (declareRealVectorValueProperty("strain_shear_vector")),
     _stress_normal_vector(declareRealVectorValueProperty("stress_normal_vector")),
     _stress_shear_vector (declareRealVectorValueProperty("stress_shear_vector"))
{ }

void
ThermalPoroElastic::computeProperties()
{
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
//rock properties
    _permeability[qp]         = _input_permeability;
    _porosity[qp]             = _input_porosity;
    _rho_r[qp]                = _input_rho_r;    
    //   _rock_specific_heat[_qp]  = _input_rock_specific_heat; The "_" on the qp was causing problems for the temp solution
    _rock_specific_heat[qp]  = _input_rock_specific_heat;
    _thermal_conductivity[qp] = _input_thermal_conductivity;
    _alpha[qp]                = _input_thermal_expansion;
    
    if(_has_temp && _has_solid_mechanics)
      _thermal_strain[qp] = _input_thermal_expansion*(_temperature[qp] - _input_t_ref);
    else
      _thermal_strain[qp] = 0.0;
    
    _youngs_modulus[qp]   = _input_youngs_modulus;
    _poissons_ratio[qp]   = _input_poissons_ratio;
    _biot_coeff[qp]       = _input_biot_coeff;

// fluid properties
    _rho_w[qp]               = _input_rho_w;
    _mu_w[qp]                = _input_mu_w;
    _c_f[qp]                 = _input_c_f; 
    _water_specific_heat[qp] = _input_water_specific_heat;
//  gravity    
    _gravity_vector[qp](0) = _gx; 
    _gravity_vector[qp](1) = _gy;
    _gravity_vector[qp](2) = _gz;
    _gravity[qp]           = _input_gravity;

    if(_has_temp && _has_variable_density)  //calculating the temperature dependent fluid density and viscosity
    {
      Real T = _temperature[qp];
      double a;
      double b;
      double c;
      
      _rho_w[qp]=1000.*(1-((pow((T-3.9863),2)/508929.2)*((T+288.9414)/(T+68.12963))));
      if (T < 0.)
       {
//         std::cerr << "T= " << T ;
//         mooseError("Temperature out of Range");
       }
    
      else if (T <= 40.)
      {
        a = 1.787E-3;
        b = (-0.03288+(1.962E-4*T))*T;
        _mu_w[qp] = a * exp(b);
      }
    
      else if (T <= 100.)
      {
        a = 1e-3;
        b = (1+(0.015512*(T-20)));
        c = -1.572;
        _mu_w[qp] = a * pow(b,c);
      }
    
      else if (T <= 300.)
      {
        a = 0.2414;
        b = 247 / (T+133.15);
        c = (a * pow(10,b));
        _mu_w[qp] = c * 1E-4;
     }
    
      else
      {
        std::cerr << "T= " << T;
        mooseError("Temperature out of Range");
      }
    }

//  compute Darcy flux and pore water velicity on q-points
    if(_has_pressure)
    {
      _darcy_params[qp] = _permeability[qp] * _rho_w[qp] / _mu_w[qp];
      _darcy_flux[qp] =  -_permeability[qp] / _mu_w[qp] * ((_grad_p[qp])+(_rho_w[qp]*_gravity[qp]*_gravity_vector[qp]));
      _pore_velocity[qp] = _darcy_flux[qp] / _porosity[qp];
    }

    if(_has_solid_mechanics)
    {
      _E  =  _youngs_modulus[qp];
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
        _strain_shear_vector[qp](1) = 0.5*(_grad_x_disp[qp](2)+_grad_z_disp[qp](0)); // s_xz
      if (_dim == 3)
        _strain_shear_vector[qp](2) = 0.5*(_grad_y_disp[qp](2)+_grad_z_disp[qp](1)); // s_yz

        _stress_normal_vector[qp](0) = _c1*_strain_normal_vector[qp](0)+_c1*_c2*_strain_normal_vector[qp](1)+_c1*_c2*_strain_normal_vector[qp](2); //tau_xx
        _stress_normal_vector[qp](1) = _c1*_c2*_strain_normal_vector[qp](0)+_c1*_strain_normal_vector[qp](1)+_c1*_c2*_strain_normal_vector[qp](2); //tau_yy
      if (_dim == 3)
        _stress_normal_vector[qp](2) = _c1*_c2*_strain_normal_vector[qp](0)+_c1*_c2*_strain_normal_vector[qp](1)+_c1*_strain_normal_vector[qp](2); //tau_zz
      
        _stress_shear_vector[qp](0) = _c1*_c3*2.0*_strain_shear_vector[qp](0); //tau_xy
      if (_dim == 3)
        _stress_shear_vector[qp](1) = _c1*_c3*2.0*_strain_shear_vector[qp](1); //tau_xz
      if (_dim == 3)
        _stress_shear_vector[qp](2) = _c1*_c3*2.0*_strain_shear_vector[qp](2); //tau_yz
    }
  }
}
