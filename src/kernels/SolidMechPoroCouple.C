#include "SolidMechPoroCouple.h"

template<>
InputParameters validParams<SolidMechPoroCouple>()
{
  InputParameters params = validParams<SolidMech>();
  return params;
}

SolidMechPoroCouple::SolidMechPoroCouple(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :SolidMech(name, moose_system, parameters),
   _pressure_var(coupled("pressure")),
   _biot_coeff(getMaterialProperty<Real>("biot_coeff")),
   _grad_pressure(coupledGradient("pressure")),
   _pressure_val(coupledValue("pressure"))
{}

