/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c)     2012 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef PTGEOTHERMAL_H
#define PTGEOTHERMAL_H

#include "Material.h"

//Forward Declarations
class PTGeothermal;

template<>
InputParameters validParams<PTGeothermal>();

class PTGeothermal : public Material
{
  public:

    PTGeothermal(const InputParameters & parameters);

  protected:

    virtual void initQpStatefulProperties();
    virtual void computeProperties();
    virtual void computeQpProperties();

    // ===============
    // local functions
    // ===============

    /* The following solely temperature-based water property functions
       are only valid if we assume a constant total compressibility.
       Thus instead we use the water-steam Equation of State, and
       pressure-temperature based formulation for the liquid-only situation
    */
    Real computeTempBasedWaterDens(Real temp);
    Real computeTempBasedWaterVisc(Real temp);
    Real computeTempBasedWaterPartialDensOverPartialTemp(Real temp);

    /*! Compute water enthalpy and density in P-T based single-phase flow
     *  @param[in]  inp          [0] pressure; [1] temperature
     *  @param[in]  inpd         2x2 identity matrix
     *  @param[out] out          [0] density;  [1] enthalpy
     *  @param[out] outd         2x2 Jacobian matrix
     *  @param[in]  nbdirs       Number of directions in differentiation
     */
    void computeWaterEquationOfStatePT_dv(Real inp[2], Real inpd[2][2],
                                          Real out[2], Real outd[2][2],
                                          int nbdirs);

    /*! Compute water viscosity in P-T based single-phase flow
     *  @param[in]  dens         water density
     *  @param[in]  temp         water temperature
     *  @param[out] visc         water viscosity
     */
    void computeViscosity(Real dens, Real temp, Real& visc);

    // ================
    // MOOSE enumerator
    // ================
    MooseEnum _istat; // user-input option for fluid propery formulation
    MooseEnum _istab; // user-input option for stabilization

    // ========================================================
    // flags to indicate the involvement of terms and equations
    // ========================================================
    bool _has_pres; // flag for pressure-based mass balance equation
    bool _has_temp; // flag for temperature-based energy balance equation

    bool _pres_dep_perm; // flag for pressure-based permeability

    // =====================
    // user-input parameters
    // =====================
    Real _ipres; // user-input reference pressure
    Real _itemp; // user-input reference temperature
    Real _iperm; // user-input rock permeability
    Real _iporo; // user-input rock porosity
    Real _irrho; // user-input rock density
    Real _irsph; // user-input rock specific heat
    Real _icomp; // user-input water compressibility
    Real _iwrho; // user-input water density
    Real _iwvis; // user-input water viscosity
    Real _iwsph; // user-input water specific heat
    Real _ithco; // user-input thermal conductivity of the reservoir
    Real _ipgdc; // user-input threshold value to include SUPG discontinuity capturing
    Real _igfor; // user-input gravity amplitude

    RealGradient _igvec; // user-input gravity unit directional vector
    RealGradient _igrdp; // user-input pressure gradient

    // ========================
    // main nonlinear variables
    // ========================
    const VariableValue & _pres; // pressure
    const VariableValue & _temp; // temperature
    const VariableValue & _pres_old; // pressure at previous time step
    const VariableValue & _temp_old; // temperature at previous time step

    const VariableGradient & _grad_pres; // pressure gradient
    const VariableGradient & _grad_temp; // temperature gradient

    // ===================
    // material properties
    // ===================
    MaterialProperty<unsigned int> & _stat; // fluid property formulation
    MaterialProperty<unsigned int> & _stab; // stabilization options

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
    MaterialProperty<Real> & _epor; // see definition in the C file
    MaterialProperty<Real> & _drop; // partial rho over partial pressure
    MaterialProperty<Real> & _drot; // partial rho over partial temperature
    MaterialProperty<Real> & _tau1; // SUPG tau1
    MaterialProperty<Real> & _tau2; // SUPG tau2

    MaterialProperty<RealGradient> & _guvec; // gravity unit directional vector
    MaterialProperty<RealGradient> & _wdflx; // water Darcy flux
    MaterialProperty<RealGradient> & _wdmfx; // water Darcy mass flux
    MaterialProperty<RealGradient> & _evelo; // energy convective velocity

    // ============================
    // stateful material properties
    // ============================
    MaterialProperty<Real> & _wrho_old; // water density at previous time step

  private:

    Real rpres, rpres_old;
    Real rtemp, rtemp_old;
    RealGradient gradp;
};
#endif //PTGEOTHERMAL_H
