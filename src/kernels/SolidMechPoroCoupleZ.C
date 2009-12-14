#include "SolidMechPoroCoupleZ.h"

template<>
InputParameters validParams<SolidMechPoroCoupleZ>()
{
  InputParameters params = validParams<SolidMechPoroCouple>();
  return params;
}

SolidMechPoroCoupleZ::SolidMechPoroCoupleZ(std::string name,
                       InputParameters parameters,
                       std::string var_name,
                       std::vector<std::string> coupled_to,
                       std::vector<std::string> coupled_as)
    :SolidMechPoroCouple(name,parameters,var_name,coupled_to,coupled_as)
  {}

Real
SolidMechPoroCoupleZ::computeQpResidual()
  {
    return _phi[_i][_qp]*(*_biot_coeff)[_qp]*_grad_pressure[_qp](2);
  }

Real
SolidMechPoroCoupleZ::computeQpOffDiagJacobian(unsigned int jvar)
  {
    if(jvar == _pressure_var)
      return _phi[_i][_qp]*(*_biot_coeff)[_qp]*_dphi[_j][_qp](2);
    return 0.0;
  }
