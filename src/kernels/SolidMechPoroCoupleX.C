#include "SolidMechPoroCoupleX.h"

template<>
InputParameters validParams<SolidMechPoroCoupleX>()
{
  InputParameters params = validParams<SolidMechPoroCouple>();
  return params;
}

SolidMechPoroCoupleX::SolidMechPoroCoupleX(std::string name,
                       InputParameters parameters,
                       std::string var_name,
                       std::vector<std::string> coupled_to,
                       std::vector<std::string> coupled_as)
    :SolidMechPoroCouple(name,parameters,var_name,coupled_to,coupled_as)
  {}

Real
SolidMechPoroCoupleX::computeQpResidual()
  {
    return _phi[_i][_qp]*(*_biot_coeff)[_qp]*_grad_pressure[_qp](0);
  }

Real
SolidMechPoroCoupleX::computeQpOffDiagJacobian(unsigned int jvar)
  {
    if(jvar == _pressure_var)
      return _phi[_i][_qp]*(*_biot_coeff)[_qp]*_dphi[_j][_qp](0);
    return 0.0;
  }
