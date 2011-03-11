#include "SolidMechPoroCoupleX.h"

template<>
InputParameters validParams<SolidMechPoroCoupleX>()
{
  InputParameters params = validParams<SolidMechPoroCouple>();
  params.addRequiredCoupledVar("pressure", "Coupled Pressure");
  return params;
}

SolidMechPoroCoupleX::SolidMechPoroCoupleX(const std::string & name, InputParameters parameters)
  :SolidMechPoroCouple(name, parameters)
   
{}

Real
SolidMechPoroCoupleX::computeQpResidual()
{
  return -_grad_phi[_i][_qp](0)*_biot_coeff[_qp]*_pressure_val[_qp];
  
}

Real
SolidMechPoroCoupleX::computeQpOffDiagJacobian(unsigned int jvar)
{
  if(jvar == _pressure_var)
    return  -_grad_phi[_i][_qp](0)*_biot_coeff[_qp]*_grad_phi[_j][_qp](0);
  return 0.0;
}
