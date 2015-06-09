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

    // ========================================================
    // flags to indicate the involvement of terms and equations
    // ========================================================
    bool _has_pres; // flag for pressure-based mass balance equation
    bool _has_temp; // flag for temperature-based energy balance equation
    bool _has_chem; // flag for chemical reaction terms

    bool _temp_dependent_weos; // falg for temperature-based EOS
    bool _pres_dependent_perm; // flag for pressure-based permeability

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
    Real _igfox; // user-input x-component of gravity directional vector
    Real _igfoy; // user-input y-component of gravity directional vector
    Real _igfoz; // user-input z-component of gravity directional vector

    // ========================
    // main nonlinear variables
    // ========================
    VariableValue & _wpres; // water pressure
    VariableValue & _wtemp; // water temperature

    VariableValue & _wpres_old; // water pressure at previous timestep
    VariableValue & _wtemp_old; // water temperature at previous timestep

    // =======================================
    // main nonliear variable gradient vectors
    // =======================================
    VariableGradient & _wgradp; // water pressure gradient

    // ==============================
    // material properties on the fly
    // ==============================
    MaterialProperty<Real> & _perm; // rock permeability
    MaterialProperty<Real> & _poro; // rock porosity
    MaterialProperty<Real> & _rrho; // rock density
    MaterialProperty<Real> & _rsph; // rock specific heat
    MaterialProperty<Real> & _comp; // water compressibility
    MaterialProperty<Real> & _wrho; // water density
    MaterialProperty<Real> & _wvis; // water viscosity
    MaterialProperty<Real> & _wtau; // water tau
    MaterialProperty<Real> & _wsph; // water specific heat
    MaterialProperty<Real> & _thco; // porous media thermal conductivity
    MaterialProperty<Real> & _gfor; // gravity magnitude

    MaterialProperty<Real> & _wDtauDpres; // water partial tau over partial pressure
    MaterialProperty<Real> & _wDrhoDtemp; // water partial rho over partial temperature
    MaterialProperty<Real> & _wDrhoDpres; // water partial rho over partial pressure

    MaterialProperty<Real> * _poro_old;   // rock porosity at previous timestep

    MaterialProperty<RealGradient> & _wdflux;   // water Darcy flux
    MaterialProperty<RealGradient> & _wdmflux;  // water Darcy mass flux
    MaterialProperty<RealGradient> & _wdmfluxp; // water Darcy mass flux due to pressure
    MaterialProperty<RealGradient> & _wdmfluxe; // water Darcy mass flux due to elevation

    MaterialProperty<RealVectorValue> & _gvec; // gravity directional unit vector
};

#endif //GEOPROCPT_H
