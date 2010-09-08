#include "SolidMechTempCoupleYFalcon.h"

template<>
InputParameters validParams<SolidMechTempCoupleYFalcon>()
{
  InputParameters params = validParams<SolidMechTempCoupleFalcon>();
  params.addCoupledVar("temperature", "TODO: add description");
  return params;
}

SolidMechTempCoupleYFalcon::SolidMechTempCoupleYFalcon(const std::string & name, MooseSystem & moose_system, InputParameters parameters)
  :SolidMechTempCoupleFalcon(name, moose_system, parameters)
{}

Real
SolidMechTempCoupleYFalcon::computeQpResidual()
{
  recomputeCouplingConstants();

  return -(_c1*(1+2*_c2)*_grad_phi[_i][_qp](1)*_thermal_strain[_qp]);
}

Real
SolidMechTempCoupleYFalcon::computeQpOffDiagJacobian(unsigned int jvar)
{
  recomputeCouplingConstants();

  if(jvar == _temp_var)
    return -(_c1*(1+2*_c2)*_grad_phi[_i][_qp](1)*_alpha[_qp]*_phi[_j][_qp]);
    
  return 0.0;
}
