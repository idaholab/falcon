#include "EGPoroFullSatTimeDerivative.h"

registerMooseObject("FalconApp", EGPoroFullSatTimeDerivative);

InputParameters
EGPoroFullSatTimeDerivative::validParams()
{
  InputParameters params = TimeDerivative::validParams();
  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");
  params.addClassDescription("Kernel = biot_coefficient*d(volumetric_strain)/dt + "
                             "(1/biot_modulus)*d(porepressure)/dt.  This is the time-derivative "
                             "for poromechanics for a single-phase, fully-saturated fluid with "
                             "constant bulk modulus");
  params.addRequiredCoupledVar("v", "Coupled variable");
  return params;
}

EGPoroFullSatTimeDerivative::EGPoroFullSatTimeDerivative(const InputParameters & parameters)
  : DerivativeMaterialInterface<TimeDerivative>(parameters),
    _v_var(dynamic_cast<MooseVariable &>(*getVar("v", 0))),
    _v(_v_var.sln()),
    _v_vals_old(coupledValueOld("v")),
    _v_id(coupled("v")),
    _volstrain(getMaterialProperty<Real>("volumetric_strain")),
    _volstrain_old(getMaterialPropertyOld<Real>("volumetric_strain")),

    _ndisp(coupledComponents("displacements")),
    _disp_var_num(_ndisp),

    _alpha(getMaterialProperty<Real>("biot_coefficient")),

    _one_over_biot_modulus(getMaterialProperty<Real>("one_over_biot_modulus")),
    _done_over_biot_modulus_dP(
        getMaterialPropertyDerivative<Real>("one_over_biot_modulus", _var.name())),
    _done_over_biot_modulus_dep(
        getMaterialPropertyDerivative<Real>("one_over_biot_modulus", "volumetric_strain"))
{
  for (unsigned i = 0; i < _ndisp; ++i)
    _disp_var_num[i] = coupled("displacements", i);
}

Real
EGPoroFullSatTimeDerivative::computeQpResidual()
{
    Real res = _one_over_biot_modulus[_qp] * (_v[_qp] - _v_vals_old[_qp]);
    return _test[_i][_qp] * res / _dt;
}

Real
EGPoroFullSatTimeDerivative::computeQpJacobian()
{
  return 0.0;
}

Real
EGPoroFullSatTimeDerivative::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real jac = 0;
  if (jvar == _v_id)
  {
    jac = _one_over_biot_modulus[_qp] * _phi[_j][_qp];
  }
  return _test[_i][_qp] * jac / _dt;
}
