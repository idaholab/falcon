#include "FluidFlow.h"

template<>
InputParameters validParams<FluidFlow>()
{
  InputParameters params = validParams<PorousMedia>(); //inherit porous medium basic properties

//the properties below were moved to auxkernels, RKP April 2011
// 
// params.addParam<Real>("density_water", 1000.0,"fluid density in Kg/m^3");
// params.addParam<Real>("viscosity_water", 0.001,"fluid dynamic viscosity in Pa.s");
// params.addParam<Real>("compressibility", 4.6e-10,"fluid compressibility in 1/Pa");
// params.addParam<bool>("temp_dependent_density", true, "Flag to call density and viscosity routine");

//these coupled variables are used to calculate some fluid mass flux related quantities only  
  params.addCoupledVar("pressure", "Use pressure here to calculate Darcy Flux and Pore Velocity");
  params.addCoupledVar("temperature", "Use temperature to calculate variable density and viscosity");
  params.addCoupledVar("density_water", "Coupled NodalAux used to calculate density");
  params.addCoupledVar("viscosity_water", "Coupled NodalAux used to calculate viscosity");
  
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
   
   _density_water(coupledValue("density_water")),   //nodal Aux
   _viscosity_water(coupledValue("viscosity_water")), //nodal Aux
     
//   _input_compressibility(getParam<Real>("compressibility")),
//delcare material properties
//  _compressibility(declareProperty<Real>("compressibility")),
   
   _tau_water(declareProperty<Real>("tau_water")),
   _darcy_flux_water(declareProperty<RealGradient>("darcy_flux_water")),
   _darcy_mass_flux_water(declareProperty<RealGradient>("darcy_mass_flux_water")),
   _darcy_mass_flux_water_pressure(declareProperty<RealGradient>("darcy_mass_flux_water_pressure")),
   _darcy_mass_flux_water_elevation(declareProperty<RealGradient>("darcy_mass_flux_water_elevation"))

{ }

void
FluidFlow::computeProperties()
{
  PorousMedia::computeProperties();
  
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {     
// fluid properties
//_compressibility[qp]      = _input_compressibility;
// _viscosity_water[qp]     = _input_viscosity_water;
//Calculate flow related quantities

    _tau_water[qp] = _permeability[qp] * _density_water[qp] / _viscosity_water[qp];
    _darcy_mass_flux_water[qp] =  -_tau_water[qp] * (_grad_p[qp]+_density_water[qp]*_gravity[qp]*_gravity_vector[qp]);
    _darcy_mass_flux_water_pressure[qp] =  (-_tau_water[qp] * _grad_p[qp]);
    _darcy_mass_flux_water_elevation[qp] = (-_tau_water[qp] * _gravity[qp] *_gravity_vector[qp]*_density_water[qp]);
    
//     std::cout << _darcy_mass_flux_water[qp]  << "\n";
    
  }
}



