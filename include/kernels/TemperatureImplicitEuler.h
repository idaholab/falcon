#include "ImplicitEuler.h"

#ifndef TEMPERATUREIMPLICITEULER
#define TEMPERATUREIMPLICITEULER

//Forward Declarations
class TemperatureImplicitEuler;

template<>
InputParameters validParams<TemperatureImplicitEuler>();

class TemperatureImplicitEuler : public ImplicitEuler
{
public:

  TemperatureImplicitEuler(std::string name, MooseSystem & moose_system, InputParameters parameters);

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
