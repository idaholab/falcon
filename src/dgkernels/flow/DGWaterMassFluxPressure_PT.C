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

<<<<<<< HEAD
//! Author:  Yidong Xia (Yidong.Xia@inl.gov)
//! Created: 08/18/2014

=======
>>>>>>> Added DG kernel: DGWaterMassFluxPressure_PT
#include "Material.h"
#include "DGWaterMassFluxPressure_PT.h"

template<>
InputParameters validParams<DGWaterMassFluxPressure_PT>()
{
  InputParameters params = validParams<DGDiffusion>();
  return params;
}

DGWaterMassFluxPressure_PT::DGWaterMassFluxPressure_PT(const std::string & name,
                                                 InputParameters parameters)
  :DGDiffusion(name, parameters),
   _tau_water(getMaterialProperty<Real>("tau_water"))
{}

Real
DGWaterMassFluxPressure_PT::computeQpResidual(Moose::DGResidualType type)
{
  return _tau_water[_qp]*DGDiffusion::computeQpResidual(type);
}

Real
DGWaterMassFluxPressure_PT::computeQpJacobian(Moose::DGJacobianType type)
{
  return _tau_water[_qp]*DGDiffusion::computeQpJacobian(type);
}

