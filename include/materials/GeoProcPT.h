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

#ifndef GEOPROCPT_H
#define GEOPROCPT_H

#include "Material.h"

//Forward Declarations
class GeoProcPT;

template<>
InputParameters validParams<GeoProcPT>();

class GeoProcPT : public Material
{
  public:

    GeoProcPT(const std::string & name, InputParameters parameters);

  protected:

    virtual void computeProperties();
    virtual void computeQpProperties();

    // ===============
    // local functions
    // ===============
    Real computeTempBasedWaterDens(Real temp);
    Real computeTempBasedWaterVisc(Real temp);
    Real computeTempBasedWaterPartialDensOverPartialTemp(Real temp);

    // ========================================================
    // flags to indicate the involvement of terms and equations
    // ========================================================
    bool _has_pres; // flag for pressure-based mass balance equation
    bool _has_temp; // flag for temperature-based energy balance equation

    bool _temp_dep_weos; // falg for temperature-based EOS
    bool _pres_dep_perm; // flag for pressure-based permeability

    // =====================
    // user-input parameters
    // =====================
    Real _iperm; // user-input rock permeability
    Real _iporo; // user-input rock porosity
    Real _irrho; // user-input rock density
    Real _irsph; // user-input rock specific heat
    Real _icomp; // user-input water compressibility
    Real _iwrho; // user-input water density
    Real _iwvis; // user-input water viscosity
    Real _iwsph; // user-input water specific heat
    Real _ithco; // user-input thermal conductivity of the reservoir
    Real _igfor; // user-input gravity amplitude

    RealGradient _igvec; // user-input gravity unit directional vector
    RealGradient _igrdp; // user-input pressure gradient

    // ========================
    // main nonlinear variables
    // ========================
    VariableValue & _pres; // water pressure
    VariableValue & _temp; // water temperature

    VariableGradient & _grad_pres; // gradient of water pressure

    // ===================
    // material properties
    // ===================
    MaterialProperty<Real> & _perm; // rock permeability
    MaterialProperty<Real> & _poro; // rock porosity
    MaterialProperty<Real> & _rrho; // rock density
    MaterialProperty<Real> & _rsph; // rock specific heat
    MaterialProperty<Real> & _comp; // water compressibility
    MaterialProperty<Real> & _wrho; // water density
    MaterialProperty<Real> & _wvis; // water viscosity
    MaterialProperty<Real> & _wtau; // water tau
    MaterialProperty<Real> & _wsph; // water specific heat
    MaterialProperty<Real> & _thco; // thermal conductivity of the reservoir
    MaterialProperty<Real> & _gfor; // gravity magnitude
    MaterialProperty<Real> & _drot; // water partial rho over partial temperature

    MaterialProperty<RealGradient> & _guvec; // gravity unit directional vector
    MaterialProperty<RealGradient> & _cgrdp; // constant pressure gradient
    MaterialProperty<RealGradient> & _wdmfp; // water Darcy mass flux due to pressure
    MaterialProperty<RealGradient> & _wdmfe; // water Darcy mass flux due to elevation
    MaterialProperty<RealGradient> & _wdmfx; // water Darcy mass flux
    MaterialProperty<RealGradient> & _wdflx; // water Darcy flux

};

#endif //GEOPROCPT_H
