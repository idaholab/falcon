#include "SolidMechSwellingSolid.h"
#include "Material.h"

template<>
InputParameters validParams<SolidMechSwellingSolid>()
{
  InputParameters params = validParams<SolidMechTempCouple>();
  params.set<Real>("component") = -1;  
  return params;
}

SolidMechSwellingSolid::SolidMechSwellingSolid(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :SolidMechTempCouple(name, moose_system, parameters),
   _solid_swelling_strain(getRealMaterialProperty("solid_swelling_strain"))
{
  _component = parameters.get<Real>("component");

  if(_component < 0)
  {
    std::cout<<"Must select a component for SolidMechSwellingSolid"<<std::endl;
    libmesh_error();
  }
}

Real
SolidMechSwellingSolid::computeQpResidual()
{
  recomputeCouplingConstants();

  return -(_c1*(1+2*_c2)*_grad_phi[_i][_qp](_component)*_solid_swelling_strain[_qp]);
}
