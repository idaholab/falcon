#include "Material.h"
#include "WaterMassFluxPressure_PT.h"

template<>
InputParameters validParams<WaterMassFluxPressure_PT>()
{
  InputParameters params = validParams<Diffusion>();
  return params;
}

WaterMassFluxPressure_PT::WaterMassFluxPressure_PT(const std::string & name,
                                             InputParameters parameters)
  :Diffusion(name, parameters),
   _tau_water(getMaterialProperty<Real>("tau_water")),
   _darcy_mass_flux_water(getMaterialProperty<RealGradient>("darcy_mass_flux_water"))
{}

Real
WaterMassFluxPressure_PT::computeQpResidual()
{
 return _tau_water[_qp]*Diffusion::computeQpResidual();
//  return -_darcy_mass_flux_water[_qp]*_grad_test[_i][_qp]; //_tau_water[_qp]*Diffusion::computeQpResidual();
}

Real
WaterMassFluxPressure_PT::computeQpJacobian()
{
  return _tau_water[_qp]*Diffusion::computeQpJacobian();
}

