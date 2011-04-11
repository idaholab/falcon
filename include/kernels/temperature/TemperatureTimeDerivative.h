#include "TimeDerivative.h"

#ifndef TEMPERATURETIMEDERIVATIVE
#define TEMPERATURETIMEDERIVATIVE

//Forward Declarations
class TemperatureTimeDerivative;

template<>
InputParameters validParams<TemperatureTimeDerivative>();

class TemperatureTimeDerivative : public TimeDerivative
{
public:

  TemperatureTimeDerivative(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  MaterialProperty<Real> & _specific_heat_water;
  MaterialProperty<Real> & _specific_heat_rock;
  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _density_water;
  MaterialProperty<Real> & _density_rock;
  
};
#endif //TEMPERATURETIMEDERIVATIVE
