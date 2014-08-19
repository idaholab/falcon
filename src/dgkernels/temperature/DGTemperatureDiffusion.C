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

//! Author:  Yidong Xia (Yidong.Xia@inl.gov)
//! Created: 08/19/2014

#include "Material.h"
#include "DGTemperatureDiffusion.h"

template<>
InputParameters validParams<DGTemperatureDiffusion>()
{
  InputParameters params = validParams<DGDiffusion>();
  return params;
}

DGTemperatureDiffusion::DGTemperatureDiffusion(const std::string & name, 
                                             InputParameters parameters)
  :DGDiffusion(name, parameters),
   _thermal_conductivity(getMaterialProperty<Real>("thermal_conductivity"))
{}

Real
DGTemperatureDiffusion::computeQpResidual(Moose::DGResidualType type)
{
  return _thermal_conductivity[_qp]*DGDiffusion::computeQpResidual(type);

}

Real
DGTemperatureDiffusion::computeQpJacobian(Moose::DGJacobianType type)
{
  return _thermal_conductivity[_qp]*DGDiffusion::computeQpJacobian(type);
}



