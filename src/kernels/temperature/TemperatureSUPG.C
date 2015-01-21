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

#include "Material.h"
#include "TemperatureSUPG.h"

template<>
InputParameters validParams<TemperatureSUPG>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<bool>("has_chem_reactions", false, "flag if chemical reactions are present");
  params.addParam<bool>("discontinuity_capturing", false, "flag for discontinuity capturing, default = false");
  params.addParam<int>("method", 0, "Choice for the discontinuity-capturing algorithm");
  params.addParam<Real>("alpha", 1.0, "Scaling coefficient");
  return params;
}

TemperatureSUPG::TemperatureSUPG(const std::string & name, InputParameters parameters) :
  Kernel(name, parameters),
  _density_water(getMaterialProperty<Real>("density_water")),
  _density_water_old(getMaterialProperty<Real>("time_old_density_water")),
  _has_chem_reactions(getParam<bool>("has_chem_reactions")),
  _porosity(getMaterialProperty<Real>("porosity")),
  _porosity_old(_has_chem_reactions ? &getMaterialPropertyOld<Real>("porosity")
                                    : &getMaterialProperty<Real>("porosity")),
  _specific_heat_water(getMaterialProperty<Real>("specific_heat_water")),
  _specific_heat_rock(getMaterialProperty<Real>("specific_heat_rock")),
  _density_rock(getMaterialProperty<Real>("density_rock")),
  _darcy_mass_flux_water(getMaterialProperty<RealGradient>("darcy_mass_flux_water")),
  _thermal_conductivity(getMaterialProperty<Real>("thermal_conductivity")),
  _u_old(valueOld()),
  _discontinuity_capturing(getParam<bool>("discontinuity_capturing")),
  _method(getParam<int>("method")),
  _alpha(getParam<Real>("alpha"))
{}

Real TemperatureSUPG::computeQpResidual()
{
  // ====================================================
  // 1. Compute the strong-form residual: strong_residual
  // ====================================================

  // Part a. time derivative
  Real sres1=(((_porosity[_qp]*_density_water[_qp]*_specific_heat_water[_qp])
              + ((1.0-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]))*_u[_qp]
             - (((*_porosity_old)[_qp]*_density_water_old[_qp]*_specific_heat_water[_qp])
                + ((1.0-(*_porosity_old)[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]))*_u_old[_qp])
            /_dt;

  // Part b. convection
  Real sres2 = _specific_heat_water[_qp] * _darcy_mass_flux_water[_qp] *_grad_u[_qp];

  // Sum up
  Real strong_residual = sres1 + sres2;

  // =================================================
  // 2. Compute the SUPG stabilization parameter: tau1
  // =================================================

  // Compute the SUPG  h size (a temporary version)
  const double hsupg = _current_elem->hmin();

  // Magnitude of the convective velocity
  Real vmod = _specific_heat_water[_qp] *
              std::sqrt( _darcy_mass_flux_water[_qp](0) * _darcy_mass_flux_water[_qp](0) +
                         _darcy_mass_flux_water[_qp](1) * _darcy_mass_flux_water[_qp](1) +
                         _darcy_mass_flux_water[_qp](2) * _darcy_mass_flux_water[_qp](2) );

  Real tau1 = hsupg / (2.0*(vmod+1.0e-7));

  // =================================================
  // 3. Compute the SUPG stabilization term: term_supg
  // =================================================

  Real term_supg = _alpha * tau1 *
                   _specific_heat_water[_qp] * _darcy_mass_flux_water[_qp] * _grad_test[_i][_qp] *
                   strong_residual;

  return term_supg;
}

Real TemperatureSUPG::computeQpJacobian()
{
  // ====================================================
  // 1. Compute the strong-form residual: strong_residual
  // ====================================================

  // Part a. time derivative
  Real sres1=((_porosity[_qp]*_density_water[_qp]*_specific_heat_water[_qp])
              + ((1.0-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]))*_phi[_j][_qp]
            /_dt;

  // Part b. convection
  Real sres2 = _specific_heat_water[_qp] * _darcy_mass_flux_water[_qp] *_grad_phi[_j][_qp];

  // Sum up
  Real strong_residual = sres1 + sres2;

  // =================================================
  // 2. Compute the SUPG stabilization parameter: tau1
  // =================================================

  // Compute the SUPG  h size (a temporary version)
  const double hsupg = _current_elem->hmin();

  // Magnitude of the convective velocity
  Real vmod = _specific_heat_water[_qp] *
              std::sqrt( _darcy_mass_flux_water[_qp](0) * _darcy_mass_flux_water[_qp](0) +
                         _darcy_mass_flux_water[_qp](1) * _darcy_mass_flux_water[_qp](1) +
                         _darcy_mass_flux_water[_qp](2) * _darcy_mass_flux_water[_qp](2) );

  Real tau1 = hsupg / (2.0*(vmod+1.0e-7));

  // =================================================
  // 3. Compute the SUPG stabilization term: term_supg
  // =================================================

  Real term_supg = _alpha * tau1 *
                   _specific_heat_water[_qp] * _darcy_mass_flux_water[_qp] * _grad_test[_i][_qp] *
                   strong_residual;

  return term_supg;
}
