#include "SolidMechSwellingSolid.h"

template<>
InputParameters validParams<SolidMechSwellingSolid>()
{
  InputParameters params;
  params.set<Real>("component") = -1;  
  return params;
}

SolidMechSwellingSolid::SolidMechSwellingSolid(std::string name,
                         InputParameters parameters,
                         std::string var_name,
                         std::vector<std::string> coupled_to,
                         std::vector<std::string> coupled_as)
    :SolidMechTempCouple(name,parameters,var_name,coupled_to,coupled_as)
  {
    _component = parameters.get<Real>("component");

    if(_component < 0)
    {
      std::cout<<"Must select a component for SolidMechSwellingSolid"<<std::endl;
      libmesh_error();
    }
  }


void
SolidMechSwellingSolid::subdomainSetup()
  {
    SolidMechTempCouple::subdomainSetup();
    
    _solid_swelling_strain = &_material->getRealProperty("solid_swelling_strain");
  }

Real
SolidMechSwellingSolid::computeQpResidual()
  {
    recomputeCouplingConstants();

    return -(_c1*(1+2*_c2)*_dphi[_i][_qp](_component)*(*_solid_swelling_strain)[_qp]);
  }
