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

/*----------------------------------------------------------------

Contributors: Yidong Xia (INL)

Descriptions: compute the residual & Jacobian of
              P-T based single-phase energy balance equation

----------------------------------------------------------------*/

#include "PTEnergyResidual.h"

registerMooseObject("FalconApp", PTEnergyResidual);

template<>
InputParameters validParams<PTEnergyResidual>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("coupled_pressure",
  "This coupled variable will be used as pressure");
  return params;
}

/*******************************************************************************
Routine: PTEnergyResidual --- constructor
Authors: Yidong Xia
*******************************************************************************/
PTEnergyResidual::PTEnergyResidual(const InputParameters & parameters):
  Kernel(parameters),
  _has_coupled_pres(isCoupled("coupled_pressure")),
  _stab(getMaterialProperty<unsigned int>("stabilization_options")),
  _thco(getMaterialProperty<Real>("thermal_conductivity")),
  _wsph(getMaterialProperty<Real>("specific_heat_water")),
  _epor(getMaterialProperty<Real>("porous_media_energy")),
  _tau1(getMaterialProperty<Real>("supg_tau1")),
  _tau2(getMaterialProperty<Real>("supg_tau2")),
  _wdmfx(getMaterialProperty<RealGradient>("darcy_mass_flux_water")),
  _evelo(getMaterialProperty<RealGradient>("energy_convective_velocity")),
  _u_dot(_is_transient ? dot() : _zero),
  _du_dot_du(_is_transient ? dotDu() : _zero),
  _pres_var(_has_coupled_pres ? coupled("coupled_pressure") : zero)
{
}


/*******************************************************************************
Routine: computeQpResidual
         --- compute residual terms on quadrature point
Authors: Yidong Xia
*******************************************************************************/
Real
PTEnergyResidual::
computeQpResidual()
{
  // contribution from the heat conduction term
  // ==========================================
  r = _thco[_qp] * _grad_u[_qp] * _grad_test[_i][_qp];

  // contribution from the heat convection term
  // ==========================================

  r += -_evelo[_qp] * _u[_qp] * _grad_test[_i][_qp];

  // contribution from the SUPG term
  // ===============================
  if (_stab[_qp] == 2)
  {
    // strong-form residual = time derivative + convection
    sres = _epor[_qp]*_u_dot[_qp] + _evelo[_qp]*_grad_u[_qp];

    // SUPG stabilization term
    r += _tau1[_qp] * _evelo[_qp] * sres * _grad_test[_i][_qp];
  }
  // contribution from the SUPG with Discontinuity Capturing term
  // ============================================================
  else if (_stab[_qp] == 3)
  {
    // strong-form residual = time derivative + convection
    sres = _epor[_qp]*_u_dot[_qp] + _evelo[_qp]*_grad_u[_qp];

    // SUPG stabilization term
    r += _tau1[_qp] * _evelo[_qp] * sres * _grad_test[_i][_qp];

    // SUPG discontinuity capturing term
    r += _tau2[_qp] * _grad_u[_qp] * std::abs(sres) * _grad_test[_i][_qp];
  }

  return r;
}


/*******************************************************************************
Routine: computeQpJacobian
         --- compute Jacobians of residual terms on quadrature point
Authors: Yidong Xia
*******************************************************************************/
Real
PTEnergyResidual::
computeQpJacobian()
{
  // contribution from the heat conduction term
  // ==========================================
  r = _thco[_qp] * _grad_phi[_j][_qp] * _grad_test[_i][_qp];

  // contribution from the heat convection term
  // ==========================================
  r += -_evelo[_qp] * _phi[_j][_qp] * _grad_test[_i][_qp];

  // contribution from the SUPG term
  // ===============================
  if (_stab[_qp] == 2)
  {
    // strong-form Jacobian = time derivative + convection
    sres = _epor[_qp]*_phi[_j][_qp]*_du_dot_du[_qp] + _evelo[_qp]*_grad_phi[_j][_qp];

    // SUPG stabilization term
    r += _tau1[_qp] * _evelo[_qp] * sres * _grad_test[_i][_qp];
  }
  // contribution from the SUPG with Discontinuity Capturing term
  // ============================================================
  else if (_stab[_qp] == 3)
  {
    // strong-form Jacobian = time derivative + convection
    sres = _epor[_qp]*_phi[_j][_qp]*_du_dot_du[_qp] + _evelo[_qp]*_grad_phi[_j][_qp];

    // SUPG stabilization term
    r += _tau1[_qp] * _evelo[_qp] * sres * _grad_test[_i][_qp];

   // (Note: contribution from discontinuity capturing term is not included
   //       because of bad convergence observed)
  }

  return r;
}
