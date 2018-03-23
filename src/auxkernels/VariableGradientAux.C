/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c)     2012 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

//! Authors: Yidong Xia (Yidong.Xia@inl.gov)
//! Created: 09/07/2014

#include "VariableGradientAux.h"

registerMooseObject("FalconApp", VariableGradientAux);

template<>
InputParameters validParams<VariableGradientAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("coupled_variable", "The gradient of this variable will be used.");
  params.addParam<int>("component",0,"Direction/component of the velocity vector (0=x, 1=y, 2=z)");
  return params;
}

VariableGradientAux::VariableGradientAux(const InputParameters & parameters)
  :AuxKernel(parameters),
   _grad_coupled_variable(coupledGradient("coupled_variable")),
   _i(getParam<int>("component"))
{}

Real
VariableGradientAux::computeValue()
{
  return _grad_coupled_variable[_qp](_i);
}
