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
//! Created: 09/18/2014

#ifndef DGFUNCTIONMATERIALDIFFUSIONBC_H
#define DGFUNCTIONMATERIALDIFFUSIONBC_H

#include "Material.h"
#include "IntegratedBC.h"

//Forward Declarations
class DGFunctionMaterialDiffusionBC;

template<>
InputParameters validParams<DGFunctionMaterialDiffusionBC>();

class DGFunctionMaterialDiffusionBC : public IntegratedBC
{
  public:

    DGFunctionMaterialDiffusionBC(const std::string & name, InputParameters parameters);

    virtual ~DGFunctionMaterialDiffusionBC() {}

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

    std::string _prop_name;
    MaterialProperty<Real> & _diff;

    /*
     * Function for variable at boundary
    */
    Function & _func;

    /*
     * Penalty parameter
    */
    Real _epsilon;

    /*
     * Stability parameter
    */
    Real _sigma;
};

#endif //DGFUNCTIONMATERIALDIFFUSIONBC_H
