#include "ImplicitBackwardDifference2.h"

#ifndef TEMPERATUREIMPLICITBACKWARDDIFERENCE2
#define TEMPERATUREIMPLICITBACKWARDDIFERENCE2

//Forward Declarations
class TemperatureImplicitBackwardDifference2;

template<>
InputParameters validParams<TemperatureImplicitBackwardDifference2>();

class TemperatureImplicitBackwardDifference2 : public ImplicitBackwardDifference2
{
public:

  TemperatureImplicitBackwardDifference2(std::string name, MooseSystem & moose_system, InputParameters parameters);

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
#endif //TEMPERATUREIMPLICITBACKWARDDIFERENCE2
