#include "SolidMechPoroCouple.h"

template<>
InputParameters validParams<SolidMechPoroCouple>()
{
  InputParameters params = validParams<SolidMechFalcon>();
  return params;
}

SolidMechPoroCouple::SolidMechPoroCouple(const std::string & name, MooseSystem & moose_system, InputParameters parameters)
  :SolidMechFalcon(name, moose_system, parameters),
   _pressure_var(coupled("pressure")),
   _biot_coeff(getMaterialProperty<Real>("biot_coeff")),
   _grad_pressure(coupledGradient("pressure")),
   _pressure_val(coupledValue("pressure"))
{}

