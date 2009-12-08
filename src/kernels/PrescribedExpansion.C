#include "PrescribedExpansion.h"

template<>
InputParameters validParams<PrescribedExpansion>()
{
  InputParameters params;
  params.set<Real>("component");
  params.set<Real>("percent");
  return params;
}

PrescribedExpansion::PrescribedExpansion(std::string name,
                      InputParameters parameters,
                      std::string var_name,
                      std::vector<std::string> coupled_to,
                      std::vector<std::string> coupled_as)
    :SolidMechTempCouple(name,parameters,var_name,coupled_to,coupled_as),
    _component(parameters.get<Real>("component")),
    _percent(parameters.get<Real>("percent"))
  {}

Real
PrescribedExpansion::computeQpResidual()
  {
    recomputeCouplingConstants();

    return -(_c1*(1+2*_c2)*_dphi[_i][_qp](_component)*0.01*_percent/3.0);
  }
