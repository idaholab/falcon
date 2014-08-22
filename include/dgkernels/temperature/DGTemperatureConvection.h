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

#ifndef DGTEMPERATURECONVECTION_H
#define DGTEMPERATURECONVECTION_H

#include "DGKernel.h"
#include "Material.h"

//Forward Declarations
class DGTemperatureConvection;

template<>
InputParameters validParams<DGTemperatureConvection>();

class DGTemperatureConvection : public DGKernel
{
  public:

    DGTemperatureConvection(const std::string & name,
                            InputParameters parameters);

  protected:

    virtual Real computeQpResidual(Moose::DGResidualType type);
    virtual Real computeQpJacobian(Moose::DGJacobianType type);

    MaterialProperty<Real> & _specific_heat_water;
    MaterialProperty<RealGradient> & _darcy_mass_flux_water;

    /**
     * A scaling factor for spectral radius
     */
    Real _alpha;
};
#endif //DGTEMPERATURECONVECTION_H
