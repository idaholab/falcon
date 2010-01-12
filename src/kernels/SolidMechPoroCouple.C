#include "SolidMechPoroCouple.h"

template<>
InputParameters validParams<SolidMechPoroCouple>()
{
  InputParameters params = validParams<SolidMech>();
  return params;
}

SolidMechPoroCouple::SolidMechPoroCouple(std::string name,
                      InputParameters parameters,
                      std::string var_name,
                      std::vector<std::string> coupled_to,
                      std::vector<std::string> coupled_as)
    :SolidMech(name,parameters,var_name,coupled_to,coupled_as),
     _pressure_var(coupled("pressure")),
     _grad_pressure(coupledGrad("pressure")),
     _pressure_val(coupledVal("pressure"))
  {}

void
SolidMechPoroCouple::subdomainSetup()
  {
    SolidMech::subdomainSetup(); 
    _biot_coeff = &_material->getRealProperty("biot_coeff");
  }

