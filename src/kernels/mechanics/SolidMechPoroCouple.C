/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "SolidMechPoroCouple.h"

template<>
InputParameters validParams<SolidMechPoroCouple>()
{
  InputParameters params = validParams<SolidMechFalcon>();
  return params;
}

SolidMechPoroCouple::SolidMechPoroCouple(const std::string & name, InputParameters parameters)
  :SolidMechFalcon(name, parameters),
   _pressure_var(coupled("pressure")),
   _biot_coeff(getMaterialProperty<Real>("biot_coeff")),
   _grad_pressure(coupledGradient("pressure")),
   _pressure_val(coupledValue("pressure"))
{}

