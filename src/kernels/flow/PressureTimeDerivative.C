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

#include "PressureTimeDerivative.h"
#include "Material.h"

template<>
InputParameters validParams<PressureTimeDerivative>()
{
    InputParameters params = validParams<TimeDerivative>();
    params.addParam<bool>("rigid_rock",true,"switch for deformable rock");
    params.addCoupledVar("x_disp", "TODO: ad description");
    params.addCoupledVar("y_disp", "TODO: ad description");
    params.addCoupledVar("z_disp", "TODO: ad description");
    return params;
}

PressureTimeDerivative::PressureTimeDerivative(const std::string & name, InputParameters parameters)
:TimeDerivative(name, parameters),
 _has_x_disp(isCoupled("x_disp")),
 _grad_x_disp(_has_x_disp ? coupledGradient("x_disp") : _grad_zero),
 _grad_x_disp_old(_has_x_disp ? coupledGradientOld("x_disp") : _grad_zero),
 _has_y_disp(isCoupled("y_disp")),
 _grad_y_disp(_has_y_disp ? coupledGradient("y_disp") : _grad_zero),
 _grad_y_disp_old(_has_y_disp ? coupledGradientOld("y_disp") : _grad_zero),
 _has_z_disp(isCoupled("z_disp")),
 _grad_z_disp(_has_z_disp ? coupledGradient("z_disp") : _grad_zero),
 _grad_z_disp_old(_has_z_disp ? coupledGradientOld("z_disp") : _grad_zero),

 _rigid_rock(getParam<bool>("rigid_rock")),
 _porosity (getMaterialProperty<Real>("material_porosity")),
 _compressibility(getMaterialProperty<Real>("compressibility")),
 _biot_coeff(getMaterialProperty<Real>("biot_coeff")),
 _biot_modulus(getMaterialProperty<Real>("biot_modulus"))
//   _porosity(coupledValue("porosity")),
//   _porosity_old(coupledValueOld("porosity"))
{}

Real
PressureTimeDerivative::computeQpResidual()
{
  if (_rigid_rock)
  {
    Real tmp1 = (_compressibility[_qp]*((_porosity[_qp]*_u[_qp])-(_porosity[_qp]*_u_old[_qp]))/_dt) * _test[_i][_qp];
    return tmp1;
  }

  Real _bulk_strain_old = _grad_x_disp_old[_qp](0) + _grad_y_disp_old[_qp](1) + _grad_z_disp_old[_qp](2);
  Real _bulk_strain_now = _grad_x_disp[_qp](0) + _grad_y_disp[_qp](1) + _grad_z_disp[_qp](2);
  Real tmp1 = 1.0e3 / _biot_modulus[_qp] * (_u[_qp]-_u_old[_qp])/_dt * _test[_i][_qp]
            + 1.0e3 * _biot_coeff[_qp] * (_bulk_strain_now - _bulk_strain_old)/_dt * _test[_i][_qp];
  return tmp1;
}

Real
PressureTimeDerivative::computeQpJacobian()
{ 
    // std::cout << _ddensitydp_H[_qp]<<"\n" ;
  if (_rigid_rock)
  {
    Real tmp1 = (_porosity[_qp]*_compressibility[_qp]*_phi[_j][_qp])*_test[_i][_qp]/_dt;  
    return tmp1;
  }
  
  Real tmp1 = 1.0e3/_biot_modulus[_qp]*_phi[_j][_qp]/_dt * _test[_i][_qp];
  return tmp1;
  
}
