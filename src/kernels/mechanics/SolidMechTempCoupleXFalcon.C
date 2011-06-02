#include "SolidMechTempCoupleXFalcon.h"

template<>
InputParameters validParams<SolidMechTempCoupleXFalcon>()
{
  InputParameters params = validParams<SolidMechTempCoupleFalcon>();
  params.addCoupledVar("temperature", "TODO: add description");
  return params;
}

SolidMechTempCoupleXFalcon::SolidMechTempCoupleXFalcon(const std::string & name, InputParameters parameters)
  :SolidMechTempCoupleFalcon(name, parameters)
{}

Real
SolidMechTempCoupleXFalcon::computeQpResidual()
{
  recomputeCouplingConstants();

  return -(_c1*(1+2*_c2)*_grad_test[_i][_qp](0)*_thermal_strain[_qp]);
}

Real
SolidMechTempCoupleXFalcon::computeQpOffDiagJacobian(unsigned int jvar)
{
  recomputeCouplingConstants();

  if(jvar == _temp_var)
    return -(_c1*(1+2*_c2)*_grad_test[_i][_qp](0)*_alpha[_qp]*_phi[_j][_qp]);
    
  return 0.0;
}
