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

#include "PTMassTimeDerivative_FD.h"

registerMooseObject("FalconApp", PTMassTimeDerivative_FD);

template<>
InputParameters validParams<PTMassTimeDerivative_FD>()
{
  InputParameters params = validParams<TimeDerivative>();
  return params;
}


/*******************************************************************************
Routine: PTMassTimeDerivative_FD -- constructor
Authors: Yidong Xia
*******************************************************************************/
PTMassTimeDerivative_FD::PTMassTimeDerivative_FD(const InputParameters & parameters):
  TimeDerivative(parameters),
  _poro(getMaterialProperty<Real>("porosity")),
  _wrho(getMaterialProperty<Real>("density_water")),
  _wrho_old(getMaterialPropertyOld<Real>("density_water")),
  _drop(getMaterialProperty<Real>("partial_rho_over_partial_pres"))
{}


/*******************************************************************************
Routine: computeQpResidual -- compute residual at quadrature point
Authors: Yidong Xia
*******************************************************************************/
Real
PTMassTimeDerivative_FD::
computeQpResidual()
{
  if(_t_step == 1)
    return _poro[_qp]*_drop[_qp]*TimeDerivative::computeQpResidual();
  else
    return _test[_i][_qp] * _poro[_qp] * (_wrho[_qp] - _wrho_old[_qp]) / _dt;
}


/*******************************************************************************
Routine: computeQpJacobian -- compute Jacobian at quadrature point
Authors: Yidong Xia
*******************************************************************************/
Real
PTMassTimeDerivative_FD::
computeQpJacobian()
{
  return _poro[_qp]*_drop[_qp]*TimeDerivative::computeQpJacobian();
}
