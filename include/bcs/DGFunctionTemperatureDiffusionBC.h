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

//! Authors: Yidong Xia (Yidong.Xia@inl.gov)
//! Created: 09/16/2014

#ifndef DGFUNCTIONTEMPERATUREDIFFUSIONBC_H
#define DGFUNCTIONTEMPERATUREDIFFUSIONBC_H

#include "Material.h"
#include "DGFunctionDiffusionDirichletBC.h"

//Forward Declarations
class DGFunctionTemperatureDiffusionBC;

template<>
InputParameters validParams<DGFunctionTemperatureDiffusionBC>();

class DGFunctionTemperatureDiffusionBC : public DGFunctionDiffusionDirichletBC
{
  public:

    DGFunctionTemperatureDiffusionBC(const std::string & name, InputParameters parameters);

    virtual ~DGFunctionTemperatureDiffusionBC() {}

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

    MaterialProperty<Real> &_thermal_conductivity;
};

#endif //DGFUNCTIONTEMPERATUREDIFFUSIONBC_H
