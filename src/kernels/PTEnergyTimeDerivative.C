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

Descriptions: time derivative of energy balance equation
              in coupled pressure-temperature based
              T-H-M-C balance equations

----------------------------------------------------------------*/

#include "PTEnergyTimeDerivative.h"

registerMooseObject("FalconApp", PTEnergyTimeDerivative);

InputParameters PTEnergyTimeDerivative::validParams()
{
  InputParameters params = TimeDerivative::validParams();
  return params;
}


/*******************************************************************************
Routine: PTEnergyTimeDerivative - constructor
Authors: Yidong Xia
*******************************************************************************/
PTEnergyTimeDerivative::PTEnergyTimeDerivative(const InputParameters & parameters):
  TimeDerivative(parameters),
   _epor(getMaterialProperty<Real>("porous_media_energy"))
{}


/*******************************************************************************
Routine: computeQpResidual
         --- time derivative in energy balance equation
Authors: Yidong Xia
*******************************************************************************/
Real
PTEnergyTimeDerivative::
computeQpResidual()
{
  return _epor[_qp]*TimeDerivative::computeQpResidual();
}


/*******************************************************************************
Routine: computeQpJacobian
         --- Jacobian of time derivative in energy balance equation
Authors: Yidong Xia
*******************************************************************************/
Real
PTEnergyTimeDerivative::
computeQpJacobian()
{
  return _epor[_qp]*TimeDerivative::computeQpJacobian();
}
