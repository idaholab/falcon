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

#ifndef PTFRACGEOTHERMAL_H
#define PTFRACGEOTHERMAL_H

#include "Material.h"

//Forward Declarations
class PTFracGeothermal;

template<>
InputParameters validParams<PTFracGeothermal>();

class PTFracGeothermal : public Material
{
  public:

    PTFracGeothermal(const InputParameters & parameters);

  protected:

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
    MooseEnum _psino; // user-input option for manual or file permeabilty input

    // ========================================================
    // flags to indicate the involvement of terms and equations
    // ========================================================
    bool _has_pres; // flag for pressure-based mass balance equation
    bool _has_temp; // flag for temperature-based energy balance equation

    bool _pres_dep_perm; // flag for pressure-based permeability
    bool _temp_dep_perm; // flag for temperature-based permeability

    // =====================
    // user-input parameters
    // =====================
    std::vector<Real> _iperm; // user-input rock permeability
    Real _iporo; // user-input rock porosity
    Real _fporo; // user-input fracture porosity
    Real _irrho; // user-input rock density
    Real _irsph; // user-input rock specific heat
    Real _icomp; // user-input water compressibility
    Real _iwrho; // user-input water density
    Real _iwvis; // user-input water viscosity
    Real _iwsph; // user-input water specific heat
    Real _ithco; // user-input thermal conductivity of the reservoir
    Real _igfor; // user-input gravity amplitude
    Real _ipres; // user-input initial reservoir pressure
    Real _ipert; // user-input permeability throttle factor
    Real _ibtem; // user-input initial reservoir temperature
    Real _iemtl; // user-input effective matrix thermal length
    Real _ilcte; // user-input linear coefficent of thermal expansion

    RealGradient _igvec; // user-input gravity unit directional vector
    RealGradient _igrdp; // user-input pressure gradient

    std::string _psfnm; // user-input permeability schedule filename

    std::vector<Real> _pschd; //scheduled time stages

    std::vector<unsigned int> _ifrac; // user-input fracture number

    // ========================
    // main nonlinear variables
    // ========================
    const VariableValue & _pres; // pressure
    const VariableValue & _temp; // temperature

    const VariableGradient & _grad_pres; // pressure gradient
    const VariableGradient & _grad_temp; // temperature gradient

    // ====================
    // coupled auxvariables
    // ====================
    const VariableValue & _fmap; // fracture map

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
    MaterialProperty<Real> & _aper; // fracture aperture
    MaterialProperty<Real> & _bape; // base fracture aperture
    MaterialProperty<Real> & _dape; // delta fracture aperture

    MaterialProperty<RealGradient> & _guvec; // gravity unit directional vector
    MaterialProperty<RealGradient> & _wdflx; // water Darcy flux
    MaterialProperty<RealGradient> & _wdmfx; // water Darcy mass flux
    MaterialProperty<RealGradient> & _evelo; // energy convective velocity

  // ==========================
  // local scheduling variables
  // ==========================

  unsigned int nperm, istag, nstag;

  private:

    RealGradient gradp;
};
#endif //PTFRACGEOTHERMAL_H
