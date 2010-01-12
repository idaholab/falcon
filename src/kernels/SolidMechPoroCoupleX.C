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
    return -_dphi[_i][_qp](0)*(*_biot_coeff)[_qp]*_pressure_val[_qp];

  }

Real
SolidMechPoroCoupleX::computeQpOffDiagJacobian(unsigned int jvar)
  {
    if(jvar == _pressure_var)
     return  -_dphi[_i][_qp](0)*(*_biot_coeff)[_qp]*_dphi[_j][_qp](0);
    return 0.0;
  }
