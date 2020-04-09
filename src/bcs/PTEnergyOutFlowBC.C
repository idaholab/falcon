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

Descriptions: P-T based single-phase energy balance equation
              outflow boundary condition

----------------------------------------------------------------*/

#include "PTEnergyOutFlowBC.h"

registerMooseObject("FalconApp", PTEnergyOutFlowBC);

InputParameters PTEnergyOutFlowBC::validParams()
{
  InputParameters params = IntegratedBC::validParams();
  return params;
}


/*******************************************************************************
Routine: PTEnergyOutFlowBC --- constructor
Authors: Yidong Xia
*******************************************************************************/
PTEnergyOutFlowBC::PTEnergyOutFlowBC(const InputParameters & parameters):
  IntegratedBC(parameters),
  _thco(getMaterialProperty<Real>("thermal_conductivity")),
  _evelo(getMaterialProperty<RealGradient>("energy_convective_velocity"))
{}


/*******************************************************************************
Routine: computeQpResidual
Authors: Yidong Xia
*******************************************************************************/
Real
PTEnergyOutFlowBC::
computeQpResidual()
{
  return
  (-_thco[_qp]*_grad_u[_qp]+_evelo[_qp]*_u[_qp])*
  _test[_i][_qp]*_normals[_qp];
}


/*******************************************************************************
Routine: computeQpJacobian
Authors: Yidong Xia
*******************************************************************************/
Real
PTEnergyOutFlowBC::
computeQpJacobian()
{
  return
  (-_thco[_qp]*_grad_phi[_j][_qp]+_evelo[_qp]*_phi[_j][_qp])*
  _test[_i][_qp]*_normals[_qp];
}
