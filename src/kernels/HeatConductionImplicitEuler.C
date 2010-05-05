#include "HeatConductionImplicitEuler.h"

template<>
InputParameters validParams<HeatConductionImplicitEuler>()
{
  InputParameters params = validParams<ImplicitEuler>();
  return params;
}

HeatConductionImplicitEuler::HeatConductionImplicitEuler(std::string name,
                                                         MooseSystem & moose_system,
                                                         InputParameters parameters)
  :ImplicitEuler(name, moose_system, parameters)
{}

void
HeatConductionImplicitEuler::subdomainSetup()
{
  _specific_heat = &_material->getRealProperty("rock_specific_heat");
  _density = &_material->getRealProperty("rho_r");
}

Real
HeatConductionImplicitEuler::computeQpResidual()
{
  return (*_specific_heat)[_qp]*(*_density)[_qp]*ImplicitEuler::computeQpResidual();
}

Real
HeatConductionImplicitEuler::computeQpJacobian()
{
  return (*_specific_heat)[_qp]*(*_density)[_qp]*ImplicitEuler::computeQpJacobian();
}
