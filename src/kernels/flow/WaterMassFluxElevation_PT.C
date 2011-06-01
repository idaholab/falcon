#include "Material.h"
#include "WaterMassFluxElevation_PT.h"

template<>
InputParameters validParams<WaterMassFluxElevation_PT>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("density_water", "Use density here to calculate Elevation Mass Flux");
  return params;
}

WaterMassFluxElevation_PT::WaterMassFluxElevation_PT(const std::string & name,
                                             InputParameters parameters)
  :Kernel(name, parameters),
   _density_water(coupledValue("density_water")),
   _tau_water(getMaterialProperty<Real>("tau_water")),
   _gravity(getMaterialProperty<Real>("gravity")),
   _gravity_vector(getMaterialProperty<RealVectorValue>("gravity_vector"))
{}

Real
WaterMassFluxElevation_PT::computeQpResidual()
{
  //return _tau_water[_qp]*_density_water[_qp]*_gravity[_qp]*_gravity_vector[_qp]*_test[_i][_qp];
  return _tau_water[_qp]*_density_water[_qp]*_gravity[_qp]*(_gravity_vector[_qp]*_grad_test[_i][_qp]);
}

Real
WaterMassFluxElevation_PT::computeQpJacobian()
{
  return 0;
}

