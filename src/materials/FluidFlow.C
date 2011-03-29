#include "FluidFlow.h"

template<>
InputParameters validParams<FluidFlow>()
{
  InputParameters params = validParams<PorousMedia>(); //inherit porous medium basic properties
  params.addParam<Real>("density_water", 1000.0,"fluid density in Kg/m^3");
  params.addParam<Real>("viscosity_water", 0.001,"fluid dynamic viscosity in Pa.s");
  params.addParam<Real>("compressibility", 4.6e-10,"fluid compressibility in 1/Pa");
  params.addParam<bool>("temp_dependent_density", true, "Flag to call density and viscosity routine");
  params.addParam<Real>("constant_temperature", 25.0, "Reference temperature for density and viscosity");


  //RKP 3/2011 this needs to get moved in to an Aux Kernel
  params.addCoupledVar("pressure", "Use pressure here to calculate Darcy Flux and Pore Velocity");
  params.addCoupledVar("temperature", "Use temperature to calculate variable density and viscosity");
  
  return params;
}

FluidFlow::FluidFlow(const std::string & name,
                     InputParameters parameters)
  :PorousMedia(name, parameters),
     _has_pressure(isCoupled("pressure")),
     _grad_p  (_has_pressure ? coupledGradient("pressure") : _grad_zero),
     _pressure(_has_pressure ? coupledValue("pressure")  : _zero),
     
     _has_temp(isCoupled("temperature")),
     _temperature(_has_temp ? coupledValue("temperature")  : _zero),
     
     _input_density_water(getParam<Real>("density_water")),
     _input_viscosity_water(getParam<Real>("viscosity_water")),
     _input_compressibility(getParam<Real>("compressibility")),
     _constant_temperature(getParam<Real>("constant_temperature")),
   
   //delcare material properties
     _density_water(declareProperty<Real>("density_water")),
     _viscosity_water(declareProperty<Real>("viscosity_water")),
     _compressibility(declareProperty<Real>("compressibility")),
   
     _has_variable_density(getParam<bool>("temp_dependent_density")),
   
     _tau_water(declareProperty<Real>("tau_water")),
     _darcy_flux_water(declareProperty<RealGradient>("darcy_flux_water")),
     _darcy_mass_flux_water(declareProperty<RealGradient>("darcy_mass_flux_water")),
     _darcy_mass_flux_water_pressure(declareProperty<RealGradient>("darcy_mass_flux_water_pressure")),
     _pore_velocity_water(declareProperty<RealGradient>("pore_velocity_water"))
{ }

void
FluidFlow::computeProperties()
{
  PorousMedia::computeProperties();
  
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {     

    // fluid properties
    _compressibility[qp]      = _input_compressibility;
    
    if ( (_has_variable_density) && (_has_temp) ) //then call the density and viscosity functions
    {
      //RKP:  Function call to "density_fun" to calc density_water
      _density_water[qp] = density_fun((_temperature)[qp]);
      _viscosity_water[qp] = viscosity_fun((_temperature)[qp]);
    }

    else if (_has_variable_density)
      {
      //RKP:  Function call to "density_fun" to calc density_water
        _density_water[qp] = density_fun(_constant_temperature);
        _viscosity_water[qp] = viscosity_fun(_constant_temperature);
    }          
    
    else //just use default water density and viscosity or values from input
    {
      _density_water[qp]        = _input_density_water;
      _viscosity_water[qp]      = _input_viscosity_water;
    }


    //std::cout << _density_water[qp] << "\n";
    
    //calculate flow related quantities
    //some of this may need to be moved to an AuxKernel
    _tau_water[qp] = _permeability[qp] * _density_water[qp] / _viscosity_water[qp];
    
    _darcy_flux_water[qp] =  -_permeability[qp] / _viscosity_water[qp] * ((_grad_p[qp])+(_density_water[qp]*_gravity[qp]*_gravity_vector[qp]));
       _darcy_mass_flux_water_pressure[qp] =  (-_tau_water[qp] * _grad_p[qp]);
       // _darcy_mass_flux_water_pressure[qp] =  (-_tau_water[qp] * _grad_p[qp]) + (-_tau_water[qp] * _density_water[qp] * _gravity[qp] * _gravity_vector[qp]);
   

    _pore_velocity_water[qp] = -_permeability[qp] / _viscosity_water[qp] * ((_grad_p[qp])+(_density_water[qp]*_gravity[qp]*_gravity_vector[qp])) / _porosity[qp];
    
//    std::cout << _darcy_flux_water[qp] << "\n";
// std::cout << _pore_velocity_water[qp] << "\n";
  }
}



//@@@@@@@@@@@@@@@@@@@@@@ begin function definitions @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//Function to calc water density, single phase conditions only
Real
FluidFlow::density_fun(Real T)
{
  Real a;
  a = 1000.*(1-((pow(((T)-3.9863),2)/508929.2)*(((T)+288.9414)/((T)+68.12963))));
   return (a);
  Real _density_water;
  _density_water=1000.*(1-((pow((T-3.9863),2)/508929.2)*((T+288.9414)/(T+68.12963))));
   return (_density_water);
}
//end density function


//Function call to calc viscosity
Real
FluidFlow::viscosity_fun(Real T)
{
  Real _viscosity_water, a, b, c, d;
  
  if (T < 0.)
    {
      std::cerr << "T= " << T ;
      mooseError("Temperature out of Range");
    }
    
    else if (T <= 40.)
    {
      a = 1.787E-3;
      b = (-0.03288+(1.962E-4*T))*T;
      _viscosity_water = a * exp(b);
     }
    
    else if (T <= 100.)
    {
      a = 1e-3;
      b = (1+(0.015512*(T-20)));
      c = -1.572;
      _viscosity_water = a * pow(b,c);
    }
    
    else if (T <= 300.)
    {
      a = 0.2414;
      b = 247 / (T+133.15);
      c = (a * pow(10,b));
      _viscosity_water = c * 1E-4;
     }
    
    else
    {
      std::cerr << "T= " << T;
      mooseError("Temperature out of Range");
    }
  return (_viscosity_water);
}
//end viscosity function

//@@@@@@@@@@@@@@@@@@@@@@ end function definitions @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
