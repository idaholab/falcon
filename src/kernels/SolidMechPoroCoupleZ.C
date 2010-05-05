#include "SolidMechPoroCoupleZ.h"

template<>
InputParameters validParams<SolidMechPoroCoupleZ>()
{
  InputParameters params = validParams<SolidMechPoroCouple>();
  return params;
}

SolidMechPoroCoupleZ::SolidMechPoroCoupleZ(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :SolidMechPoroCouple(name, moose_system, parameters)
{}

Real
SolidMechPoroCoupleZ::computeQpResidual()
{
//    return _phi[_i][_qp]*(*_biot_coeff)[_qp]*_grad_pressure[_qp](2);
  return -_dphi[_i][_qp](2)*(*_biot_coeff)[_qp]*_pressure_val[_qp];
}

Real
SolidMechPoroCoupleZ::computeQpOffDiagJacobian(unsigned int jvar)
{
  if(jvar == _pressure_var)
//      return _phi[_i][_qp]*(*_biot_coeff)[_qp]*_dphi[_j][_qp](2);
    return  -_dphi[_i][_qp](2)*(*_biot_coeff)[_qp]*_dphi[_j][_qp](2);      
  return 0.0;
}
