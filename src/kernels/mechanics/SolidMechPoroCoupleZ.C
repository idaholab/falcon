#include "SolidMechPoroCoupleZ.h"

template<>
InputParameters validParams<SolidMechPoroCoupleZ>()
{
  InputParameters params = validParams<SolidMechPoroCouple>();
  params.addRequiredCoupledVar("pressure", "Coupled Pressure");
  return params;
}

SolidMechPoroCoupleZ::SolidMechPoroCoupleZ(const std::string & name, InputParameters parameters)
  :SolidMechPoroCouple(name, parameters)
{}

Real
SolidMechPoroCoupleZ::computeQpResidual()
{
    return _test[_i][_qp]*_biot_coeff[_qp]*_grad_pressure[_qp](2);
//  return -_grad_test[_i][_qp](2)*_biot_coeff[_qp]*_pressure_val[_qp];
}

Real
SolidMechPoroCoupleZ::computeQpOffDiagJacobian(unsigned int jvar)
{
  if(jvar == _pressure_var)
    return _test[_i][_qp]*_biot_coeff[_qp]*_grad_phi[_j][_qp](2);
//  return  -_grad_test[_i][_qp](2)*_biot_coeff[_qp]*_grad_phi[_j][_qp](2);
  return 0.0;
}
