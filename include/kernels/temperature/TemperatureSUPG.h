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
//! Created: 09/11/2014

#ifndef TEMPERATURESUPG_H
#define TEMPERATURESUPG_H

#include "Material.h"
#include "Kernel.h"

class TemperatureSUPG;

template<>
InputParameters validParams<TemperatureSUPG>();

class TemperatureSUPG : public Kernel
{
  public:

    TemperatureSUPG(const std::string & name, InputParameters parameters);

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

    const MaterialProperty<Real> & _density_water;
    const MaterialProperty<Real> & _density_water_old;
    bool _has_chem_reactions;
    const MaterialProperty<Real> & _porosity;
    const MaterialProperty<Real> * _porosity_old;
    const MaterialProperty<Real> & _specific_heat_water;
    const MaterialProperty<Real> & _specific_heat_rock;
    const MaterialProperty<Real> & _density_rock;
    const MaterialProperty<RealGradient> & _darcy_mass_flux_water;
    const MaterialProperty<Real> &_thermal_conductivity;
    VariableValue & _u_old;

    /**
     * Flag for discontinuity capturing
     */
    bool _discontinuity_capturing;

    /**
     * Choice for the discontinuity-capturing method
     */
    int _method;

    /**
     * A scaling coefficient
     */
    Real _alpha;
};

#endif //TEMPERATURESUPG_H
