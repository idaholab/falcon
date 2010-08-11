#include "SolidMechTempCoupleXFalcon.h"

template<>
InputParameters validParams<SolidMechTempCoupleXFalcon>()
{
  InputParameters params = validParams<SolidMechTempCoupleFalcon>();
  params.addCoupledVar("temperature", "TODO: add description");
  return params;
}

SolidMechTempCoupleXFalcon::SolidMechTempCoupleXFalcon(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :SolidMechTempCoupleFalcon(name, moose_system, parameters)
{}

Real
SolidMechTempCoupleXFalcon::computeQpResidual()
{
  recomputeCouplingConstants();

  return -(_c1*(1+2*_c2)*_grad_phi[_i][_qp](0)*_thermal_strain[_qp]);
}

Real
SolidMechTempCoupleXFalcon::computeQpOffDiagJacobian(unsigned int jvar)
{
  recomputeCouplingConstants();

  if(jvar == _temp_var)
    return -(_c1*(1+2*_c2)*_grad_phi[_i][_qp](0)*_alpha[_qp]*_phi[_j][_qp]);
    
  return 0.0;
}
