#include "Material.h"
#include "CoupledPorosityNodalAux.h"

template<>
InputParameters validParams<CoupledPorosityNodalAux>()
{
  InputParameters params = validParams<AuxKernel>();
//  params.addRequiredCoupledVar("temperature", "Use temperature to calculate variable density and viscosity");
//  params.addParam<bool>("temp_dependent_viscosity", true, "Flag to call density and viscosity routine");


  params.addParam<Real>("porosity", 0.3,"Initial reservoir porosity");
  return params;
}

CoupledPorosityNodalAux::CoupledPorosityNodalAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
   //  _temperature(coupledValue("temperature")),
   _input_porosity(getParam<Real>("porosity"))
   
   //   _input_porosity(getMaterialProperty<Real>("material_porosity"))
   
//   _has_variable_viscosity(getParam<bool>("temp_dependent_viscosity"))

{}


Real
CoupledPorosityNodalAux::computeValue()
{
return _input_porosity;
}


