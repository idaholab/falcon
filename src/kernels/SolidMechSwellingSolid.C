#include "SolidMechSwellingSolid.h"
#include "Material.h"

template<>
InputParameters validParams<SolidMechSwellingSolid>()
{
  InputParameters params = validParams<SolidMechTempCoupleFalcon>();
  params.set<Real>("component") = -1;  
  return params;
}

SolidMechSwellingSolid::SolidMechSwellingSolid(const std::string & name, InputParameters parameters)
  :SolidMechTempCoupleFalcon(name, parameters),
   _solid_swelling_strain(getMaterialProperty<Real>("solid_swelling_strain"))
{
  _component = getParam<Real>("component");

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
