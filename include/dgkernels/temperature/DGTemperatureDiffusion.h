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

#ifndef DGTEMPERATUREDIFFUSION
#define DGTEMPERATUREDIFFUSION

#include "DGDiffusion.h"
#include "Material.h"

class DGTemperatureDiffusion;

template<>
InputParameters validParams<DGTemperatureDiffusion>();

class DGTemperatureDiffusion : public DGDiffusion
{
public:

  DGTemperatureDiffusion(const std::string & name, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual(Moose::DGResidualType type);
  virtual Real computeQpJacobian(Moose::DGJacobianType type);

  MaterialProperty<Real> &_thermal_conductivity;
  
};
#endif //DGTEMPERATUREDIFFUSION
