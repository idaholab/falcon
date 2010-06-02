#include "SolidMechSwellingGas.h"

template<>
InputParameters validParams<SolidMechSwellingGas>()
{
  InputParameters params = validParams<Kernel>();
  params.set<Real>("burnup") = 0;
  params.set<Real>("component") = -1;
  
  return params;
}

SolidMechSwellingGas::SolidMechSwellingGas(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),
   _temp(coupledVal("temp")),
   _density(getRealMaterialProperty("density"))
{
  _burnup = parameters.get<Real>("burnup");
  _component = parameters.get<Real>("component");
  
  if(_component < 0)
  {
    std::cout<<"Must select a component for SolidMechSwellingGas"<<std::endl;
    libmesh_error();
  }    
}

Real
SolidMechSwellingGas::computeQpResidual()
{
  Real density = _density[_qp];

  return -_dphi[_i][_qp](_component)*
    (
      2.25e-31*density*
      std::pow(2800-_temp[_qp],11.73)*
      exp(-0.0162*(2800-_temp[_qp]))*
      exp(-0.021*density*_burnup)
      )
    *_burnup;
}
