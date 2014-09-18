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
//! Created: 08/22/2014

#ifndef DGFUNCTIONTEMPRATURECONVECTIONINFLOWBC_H
#define DGFUNCTIONTEMPRATURECONVECTIONINFLOWBC_H

#include "IntegratedBC.h"
#include "Material.h"

//Forward Declarations
class DGFunctionTemperatureConvectionInflowBC;

template<>
InputParameters validParams<DGFunctionTemperatureConvectionInflowBC>();

class DGFunctionTemperatureConvectionInflowBC : public IntegratedBC
{
  public:

    DGFunctionTemperatureConvectionInflowBC(const std::string & name,
                                         InputParameters parameters);

    virtual ~DGFunctionTemperatureConvectionInflowBC() {}

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

    MaterialProperty<Real> & _specific_heat_water;
    MaterialProperty<RealGradient> & _darcy_mass_flux_water;

    Function & _func;
};

#endif //DGFUNCTIONTEMPERATURECONVECTIONINFLOWBC_H
