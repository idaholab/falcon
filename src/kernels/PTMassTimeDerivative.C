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

/*----------------------------------------------------------------

Contributors: Yidong Xia (INL)

Descriptions: time derivative of mass balance equation
              in coupled pressure-temperature based
              T-H-M-C balance equations

----------------------------------------------------------------*/

#include "PTMassTimeDerivative.h"

registerMooseObject("FalconApp", PTMassTimeDerivative);

template<>
InputParameters validParams<PTMassTimeDerivative>()
{
  InputParameters params = validParams<TimeDerivative>();
  return params;
}


/*******************************************************************************
Routine: PTMassTimeDerivative -- constructor
Authors: Yidong Xia
*******************************************************************************/
PTMassTimeDerivative::PTMassTimeDerivative(const InputParameters & parameters):
  TimeDerivative(parameters),
  _poro(getMaterialProperty<Real>("porosity")),
  _wrho(getMaterialProperty<Real>("density_water")),
  _drop(getMaterialProperty<Real>("partial_rho_over_partial_pres"))
{}


/*******************************************************************************
Routine: computeQpResidual -- compute residual at quadrature point
Authors: Yidong Xia
*******************************************************************************/
Real
PTMassTimeDerivative::
computeQpResidual()
{
  return _poro[_qp]*_drop[_qp]*TimeDerivative::computeQpResidual();
}


/*******************************************************************************
Routine: computeQpJacobian -- compute Jacobian at quadrature point
Authors: Yidong Xia
*******************************************************************************/
Real
PTMassTimeDerivative::
computeQpJacobian()
{
  return _poro[_qp]*_drop[_qp]*TimeDerivative::computeQpJacobian();
}
