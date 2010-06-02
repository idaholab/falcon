#include "SolidMechPoroCoupleX.h"

template<>
InputParameters validParams<SolidMechPoroCoupleX>()
{
  InputParameters params = validParams<SolidMechPoroCouple>();
  return params;
}

SolidMechPoroCoupleX::SolidMechPoroCoupleX(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :SolidMechPoroCouple(name, moose_system, parameters)
{}

Real
SolidMechPoroCoupleX::computeQpResidual()
{
  return -_dphi[_i][_qp](0)*_biot_coeff[_qp]*_pressure_val[_qp];
  
}

Real
SolidMechPoroCoupleX::computeQpOffDiagJacobian(unsigned int jvar)
{
  if(jvar == _pressure_var)
    return  -_dphi[_i][_qp](0)*_biot_coeff[_qp]*_dphi[_j][_qp](0);
  return 0.0;
}
