#include "TimeDerivative.h"

#ifndef TEMPERATURETIMEDERIVATIVESOLID
#define TEMPERATURETIMEDERIVATIVESOLID

//Forward Declarations
class TemperatureTimeDerivativeSolid;

template<>
InputParameters validParams<TemperatureTimeDerivativeSolid>();

class TemperatureTimeDerivativeSolid : public TimeDerivative
{
public:

  TemperatureTimeDerivativeSolid(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

//  VariableValue  & _density_water;
//  VariableValue & _density_water_old;
  
  VariableValue  & _porosity;
  VariableValue  & _porosity_old;

//  MaterialProperty<Real> & _specific_heat_water;
  MaterialProperty<Real> & _specific_heat_rock;
  MaterialProperty<Real> & _density_rock;
  
};
#endif //TEMPERATURETIMEDERIVATIVESOLID
