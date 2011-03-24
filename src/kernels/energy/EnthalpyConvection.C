#include "Material.h"
#include "EnthalpyConvection.h"

template<>
InputParameters validParams<EnthalpyConvection>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("pressure", "TODO: add description");
  return params;
}

EnthalpyConvection::EnthalpyConvection(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),

   
   _grad_p(coupledGradient("pressure")),
   _enthalpy_saturated_water(getMaterialProperty<Real>("enthalpy_saturated_water")),
   _enthalpy_saturated_steam(getMaterialProperty<Real>("enthalpy_saturated_steam")),
   _tau_steam(getMaterialProperty<Real>("tau_steam")),
   _tau_water(getMaterialProperty<Real>("tau_water"))
   
{}

Real EnthalpyConvection::computeQpResidual()
{
  
  return ((_enthalpy_saturated_steam[_qp]*_tau_steam[_qp])+(_enthalpy_saturated_water[_qp]*_tau_water[_qp]))* _grad_test[_i][_qp]*_grad_p[_qp];

}

Real EnthalpyConvection::computeQpJacobian()
{
  return 0;
 
}
