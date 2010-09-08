#include "SolidMechPoroCoupleY.h"

template<>
InputParameters validParams<SolidMechPoroCoupleY>()
{
  InputParameters params = validParams<SolidMechPoroCouple>();
  return params;
}

SolidMechPoroCoupleY::SolidMechPoroCoupleY(const std::string & name, MooseSystem & moose_system, InputParameters parameters)
  :SolidMechPoroCouple(name, moose_system, parameters)
{}

Real
SolidMechPoroCoupleY::computeQpResidual()
{
  return -_grad_phi[_i][_qp](1)*_biot_coeff[_qp]*_pressure_val[_qp];
//    return _phi[_i][_qp]*_biot_coeff[_qp]*_grad_pressure[_qp](1);
}

Real
SolidMechPoroCoupleY::computeQpOffDiagJacobian(unsigned int jvar)
{
  if(jvar == _pressure_var)
//      return _phi[_i][_qp]*_biot_coeff[_qp]*_dphi[_j][_qp](1);
    return  -_grad_phi[_i][_qp](1)*_biot_coeff[_qp]*_grad_phi[_j][_qp](1);
  return 0.0;
}
