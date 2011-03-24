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

  TemperatureImplicitEuler(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  MaterialProperty<Real> & _specific_heat_water;
  MaterialProperty<Real> & _specific_heat_rock;
  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _density_water;
  MaterialProperty<Real> & _density_rock;
  
};
#endif //TEMPERATUREIMPLICITEULER
