#include "Material.h"
#include "EnthalpyConvectionSteam.h"

template<>
InputParameters validParams<EnthalpyConvectionSteam>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

EnthalpyConvectionSteam::EnthalpyConvectionSteam(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),

   _darcy_flux_steam(getMaterialProperty<RealGradient>("darcy_flux_steam")),
   _Genthalpy_saturated_steam(getMaterialProperty<RealGradient>("grad_enthalpy_saturated_steam")),
   _density_steam(getMaterialProperty<Real>("density_steam"))

{}

Real EnthalpyConvectionSteam::computeQpResidual()
{


  return  _darcy_flux_steam[_qp]*_density_steam[_qp]*_test[_i][_qp]*_Genthalpy_saturated_steam[_qp];
}

Real EnthalpyConvectionSteam::computeQpJacobian()
{

  return 0.0;
  
}
