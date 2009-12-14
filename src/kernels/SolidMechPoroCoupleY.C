#include "SolidMechPoroCoupleY.h"

template<>
InputParameters validParams<SolidMechPoroCoupleY>()
{
  InputParameters params = validParams<SolidMechPoroCouple>();
  return params;
}

SolidMechPoroCoupleY::SolidMechPoroCoupleY(std::string name,
                       InputParameters parameters,
                       std::string var_name,
                       std::vector<std::string> coupled_to,
                       std::vector<std::string> coupled_as)
    :SolidMechPoroCouple(name,parameters,var_name,coupled_to,coupled_as)
  {}

Real
SolidMechPoroCoupleY::computeQpResidual()
  {
    return _phi[_i][_qp]*(*_biot_coeff)[_qp]*_grad_pressure[_qp](1);
  }

Real
SolidMechPoroCoupleY::computeQpOffDiagJacobian(unsigned int jvar)
  {
    if(jvar == _pressure_var)
      return _phi[_i][_qp]*(*_biot_coeff)[_qp]*_dphi[_j][_qp](1);
    
    return 0.0;
  }
