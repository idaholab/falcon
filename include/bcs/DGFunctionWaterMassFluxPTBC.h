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

#ifndef DGFUNCTIONWATERMASSFLUXPTBC_H
#define DGFUNCTIONWATERMASSFLUXPTBC_H

#include "Material.h"
#include "DGFunctionDiffusionDirichletBC.h"

//Forward Declarations
class DGFunctionWaterMassFluxPTBC;

template<>
InputParameters validParams<DGFunctionWaterMassFluxPTBC>();

class DGFunctionWaterMassFluxPTBC : public DGFunctionDiffusionDirichletBC
{
  public:

    DGFunctionWaterMassFluxPTBC(const std::string & name, InputParameters parameters);

    virtual ~DGFunctionWaterMassFluxPTBC() {}

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

    MaterialProperty<Real> & _tau_water;
};

#endif //DGFUNCTIONWATERMASSFLUXPTBC_H
