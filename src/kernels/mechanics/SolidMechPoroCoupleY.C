
#include "SolidMechPoroCoupleY.h"

template<>
InputParameters validParams<SolidMechPoroCoupleY>()
{
  InputParameters params = validParams<SolidMechPoroCouple>();
  params.addRequiredCoupledVar("pressure", "Coupled Pressure");
  return params;
}

SolidMechPoroCoupleY::SolidMechPoroCoupleY(const std::string & name, InputParameters parameters)
  :SolidMechPoroCouple(name, parameters)
{}

Real
SolidMechPoroCoupleY::computeQpResidual()
{
//  return -_grad_test[_i][_qp](1)*_biot_coeff[_qp]*_pressure_val[_qp];
  return _test[_i][_qp]*_biot_coeff[_qp]*_grad_pressure[_qp](1);
}

Real
SolidMechPoroCoupleY::computeQpOffDiagJacobian(unsigned int jvar)
{
  if(jvar == _pressure_var)
    return _test[_i][_qp]*_biot_coeff[_qp]*_grad_phi[_j][_qp](1);
//  return  -_grad_test[_i][_qp](1)*_biot_coeff[_qp]*_phi[_j][_qp];
  return 0.0;
}
