/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

//! Author:  Yidong Xia (Yidong.Xia@inl.gov)
//! Created: 08/22/2014

#ifndef DGFUNCTIONCONVECTIONBC_H
#define DGFUNCTIONCONVECTIONBC_H

#include "IntegratedBC.h"
#include "Material.h"

//Forward Declarations
class DGFunctionConvectionBC;

template<>
InputParameters validParams<DGFunctionConvectionBC>();

class DGFunctionConvectionBC : public IntegratedBC
{
  public:

    DGFunctionConvectionBC(const std::string & name, InputParameters parameters);

    virtual ~DGFunctionConvectionBC() {}

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

    MaterialProperty<Real> & _specific_heat_water;
    MaterialProperty<RealGradient> & _darcy_mass_flux_water;

  private:

    Function & _func;
};

#endif //DGFUNCTIONCONVECTIONBC_H
