#include "SolidMechTempCoupleZFalcon.h"

template<>
InputParameters validParams<SolidMechTempCoupleZFalcon>()
{
  InputParameters params = validParams<SolidMechTempCoupleFalcon>();
  params.addCoupledVar("temperature", "TODO: add description");
  return params;
}

SolidMechTempCoupleZFalcon::SolidMechTempCoupleZFalcon(const std::string & name, InputParameters parameters)
  :SolidMechTempCoupleFalcon(name, parameters)
{}

Real
SolidMechTempCoupleZFalcon::computeQpResidual()
{
  recomputeCouplingConstants();

  return -(_c1*(1+2*_c2)*_grad_phi[_i][_qp](2)*_thermal_strain[_qp]);
}

Real
SolidMechTempCoupleZFalcon::computeQpOffDiagJacobian(unsigned int jvar)
{
  recomputeCouplingConstants();
        
  if(jvar == _temp_var)
    return -(_c1*(1+2*_c2)*_grad_phi[_i][_qp](2)*_alpha[_qp]*_phi[_j][_qp]);
    
  return 0.0;
}
