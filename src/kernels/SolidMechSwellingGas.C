#include "SolidMechSwellingGas.h"

template<>
InputParameters validParams<SolidMechSwellingGas>()
{
  InputParameters params;
  params.set<Real>("burnup") = 0;
  params.set<Real>("component") = -1;
  
  return params;
}

SolidMechSwellingGas::SolidMechSwellingGas(std::string name,
                       InputParameters parameters,
                       std::string var_name,
                       std::vector<std::string> coupled_to,
                       std::vector<std::string> coupled_as)
    :Kernel(name,parameters,var_name,true,coupled_to,coupled_as),
     _temp(coupledVal("temp"))
  {
    _burnup = parameters.get<Real>("burnup");
    _component = parameters.get<Real>("component");

    if(_component < 0)
    {
      std::cout<<"Must select a component for SolidMechSwellingGas"<<std::endl;
      libmesh_error();
    }    
  }

void
SolidMechSwellingGas::subdomainSetup()
  {
    _density = &_material->getRealProperty("density");
  }

Real
SolidMechSwellingGas::computeQpResidual()
  {
    Real density = (*_density)[_qp];

    return -_dphi[_i][_qp](_component)*
      (
        2.25e-31*density*
        std::pow(2800-_temp[_qp],11.73)*
        exp(-0.0162*(2800-_temp[_qp]))*
        exp(-0.021*density*_burnup)
      )
      *_burnup;
  }
