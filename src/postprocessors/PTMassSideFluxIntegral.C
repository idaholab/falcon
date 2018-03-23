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

Descriptions: calculate P-T based total mass flux on a side

----------------------------------------------------------------*/

#include "PTMassSideFluxIntegral.h"

registerMooseObject("FalconApp", PTMassSideFluxIntegral);

template<>
InputParameters validParams<PTMassSideFluxIntegral>()
{
  InputParameters params = validParams<SideIntegralVariablePostprocessor>();
  return params;
}


/*******************************************************************************
Routine: PTMassSideFluxIntegral --- constructor
Authors: Yidong Xia
*******************************************************************************/
PTMassSideFluxIntegral::PTMassSideFluxIntegral(const InputParameters & parameters):
  SideIntegralVariablePostprocessor(parameters),
  _wdmfx(getMaterialProperty<RealGradient>("darcy_mass_flux_water"))
{}


/*******************************************************************************
Routine: computeQpResidual
Authors: Yidong Xia
*******************************************************************************/
Real
PTMassSideFluxIntegral::computeQpIntegral()
{
  return _wdmfx[_qp]*_normals[_qp];
}

