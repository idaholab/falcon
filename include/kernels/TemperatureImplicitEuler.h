#include "ImplicitEuler.h"

#ifndef TEMPERATUREIMPLICITEULER
#define TEMPERATUREIMPLICITEULER



class TemperatureImplicitEuler : public ImplicitEuler
{
public:

  TemperatureImplicitEuler(std::string name,
                     Parameters parameters,
                     std::string var_name,
                     std::vector<std::string> coupled_to,
                     std::vector<std::string> coupled_as);

  virtual void subdomainSetup();

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  std::vector<Real> * _water_specific_heat;
  std::vector<Real> * _rock_specific_heat;
  std::vector<Real> * _porosity;
  std::vector<Real> * _rho_w;
  std::vector<Real> * _rho_r;
  
};
#endif //TEMPERATUREIMPLICITEULER
