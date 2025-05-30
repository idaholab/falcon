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
              P-T based single-phase mass balance equation

----------------------------------------------------------------*/

#include "PTMassResidual.h"

using namespace libMesh;

registerMooseObject("FalconApp", PTMassResidual);

InputParameters PTMassResidual::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addCoupledVar("coupled_temperature",
                       "This coupled variable will be used as temperature");
  return params;
}

/*******************************************************************************
Routine: PTMassResidual -- constructor
Authors: Yidong Xia
*******************************************************************************/
PTMassResidual::PTMassResidual(const InputParameters &parameters) : Kernel(parameters),
                                                                    _has_coupled_temp(isCoupled("coupled_temperature")),
                                                                    _wrho(getMaterialProperty<Real>("density_water")),
                                                                    _wtau(getMaterialProperty<Real>("tau_water")),
                                                                    _gfor(getMaterialProperty<Real>("gravity")),
                                                                    _drot(getMaterialProperty<Real>("partial_rho_over_partial_temp")),
                                                                    _perm(getMaterialProperty<Real>("permeability")),
                                                                    _dkdp(getMaterialProperty<Real>("partial_perm_over_partial_pres")),
                                                                    _guvec(getMaterialProperty<RealGradient>("gravity_direction")),
                                                                    _wdmfx(getMaterialProperty<RealGradient>("darcy_mass_flux_water")),
                                                                    _temp_var(_has_coupled_temp ? coupled("coupled_temperature") : zero)
{
}

/*******************************************************************************
Routine: computeQpResidual -- compute residual at quadrature point
Authors: Yidong Xia
*******************************************************************************/
Real PTMassResidual::
    computeQpResidual()
{
  return -_wdmfx[_qp] * _grad_test[_i][_qp];
}

/*******************************************************************************
Routine: computeQpJacobian -- compute Jacobian at quadrature point
Authors: Yidong Xia
*******************************************************************************/
Real PTMassResidual::
    computeQpJacobian()
{
  Real r = 0.0;

  // contribution from Darcy mass flux due to pressure gradient
  r += (_wtau[_qp] * _wrho[_qp] * _grad_phi[_j][_qp] +
        _dkdp[_qp] / _perm[_qp] * _wdmfx[_qp]) *
       _grad_test[_i][_qp];

  // contribution from Darcy mass flux due to elevation
  // omitted

  return r;
}

/*******************************************************************************
Routine: computeQpOffDiagJacobian
         -- compute off-diagonal entries of Jacobian at quadrature point
Authors: Yidong Xia
*******************************************************************************/
Real PTMassResidual::
    computeQpOffDiagJacobian(unsigned int jvar)
{
  Real r = 0.0;

  // contribution from Darcy mass flux due to pressure gradient
  // omitted

  // contribution from Darcy mass flux due to elevation
  if (jvar == _temp_var && _has_coupled_temp)
    r += 2.0 * _wtau[_qp] * _wrho[_qp] * _drot[_qp] * _phi[_j][_qp] *
         _gfor[_qp] * (_guvec[_qp] * _grad_test[_i][_qp]);

  return r;
}
