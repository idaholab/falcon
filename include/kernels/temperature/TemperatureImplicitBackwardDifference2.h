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

  TemperatureImplicitBackwardDifference2(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  MaterialProperty<Real> & _specific_heat_water;
  MaterialProperty<Real> & _specific_heat_rock;
  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _density_water;
  MaterialProperty<Real> & _density_rock;
  
};
#endif //TEMPERATUREIMPLICITBACKWARDDIFERENCE2
