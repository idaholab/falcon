#include "Material.h"
#include "EnthalpyConvectionWater.h"

template<>
InputParameters validParams<EnthalpyConvectionWater>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

EnthalpyConvectionWater::EnthalpyConvectionWater(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),

   _darcy_flux_water(getMaterialProperty<RealGradient>("darcy_flux_water")),
   _Genthalpy_saturated_water(getMaterialProperty<RealGradient>("grad_enthalpy_saturated_water")),
   _density_water(getMaterialProperty<Real>("density_water"))

{}

Real EnthalpyConvectionWater::computeQpResidual()
{
  
   return _darcy_flux_water[_qp]*_density_water[_qp]*_test[_i][_qp]*_Genthalpy_saturated_water[_qp];

}

Real EnthalpyConvectionWater::computeQpJacobian()
{
  return 0.0;
}
