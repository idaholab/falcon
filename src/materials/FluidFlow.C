#include "FluidFlow.h"

template<>
InputParameters validParams<FluidFlow>()
{
  InputParameters params = validParams<PorousMedia>(); //inherit porous medium basic properties
  params.addParam<Real>("rho_w",1000.0,"fluid density in Kg/m^3");
  params.addParam<Real>("mu_w",0.001,"fluid dynamic viscosity in Pa.s");
  params.addParam<Real>("c_f", 4.6e-10,"fluid compressibility in 1/Pa");
  params.set<bool>("temp_dependent_density")  = true;
  return params;
}

FluidFlow::FluidFlow(const std::string & name,
                     MooseSystem & moose_system,
                     InputParameters parameters)
  :PorousMedia(name, moose_system, parameters),
     _has_pressure(isCoupled("pressure")),
     _grad_p  (_has_pressure ? coupledGradient("pressure") : _grad_zero),
     _pressure(_has_pressure ? coupledValue("pressure")  : _zero),
     
     _has_temp(isCoupled("temperature")),
     _temperature(_has_temp ? coupledValue("temperature")  : _zero),
     
     _input_rho_w(getParam<Real>("rho_w")),
     _input_mu_w(getParam<Real>("mu_w")),
     _input_c_f(getParam<Real>("c_f")),

   //delcare material properties
     _rho_w(declareProperty<Real>("rho_w")),
     _mu_w(declareProperty<Real>("mu_w")),
     _c_f(declareProperty<Real>("c_f")),
     _has_variable_density(getParam<bool>("temp_dependent_density")),
     _darcy_params_w(declareProperty<Real>("darcy_params_w")),
     _darcy_flux_w(declareProperty<RealGradient>("darcy_flux_w")),
     _pore_velocity_w(declareProperty<RealGradient>("pore_velocity_w"))
{ }

void
FluidFlow::computeProperties()
{
  PorousMedia::computeProperties();
  
  for(unsigned int qp=0; qp<_n_qpoints; qp++)
  {     
// fluid properties
    _rho_w[qp]               = _input_rho_w;
    _mu_w[qp]                = _input_mu_w;
    _c_f[qp]                 = _input_c_f; 

// temperature dependent fluid density & viscosity
/*    if(_has_temp && _has_variable_density) 
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
*/    
//  compute Darcy flux and pore water velicity on q-points
//    if(_has_pressure)
//    {
      _darcy_params_w[qp] = _permeability[qp] * _rho_w[qp] / _mu_w[qp];
      _darcy_flux_w[qp] =  -_permeability[qp] / _mu_w[qp] * ((_grad_p[qp])+(_rho_w[qp]*_gravity[qp]*_gravity_vector[qp]));
      _pore_velocity_w[qp] = _darcy_flux_w[qp] / _porosity[qp];
//    }

//    std::cout<<"darcy_params "<<_darcy_params_w[qp]<<".\n";
    
  }
}
