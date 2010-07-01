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

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  MaterialProperty<Real> & _water_specific_heat;
  MaterialProperty<Real> & _rock_specific_heat;
  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _rho_w;
  MaterialProperty<Real> & _rho_r;
  
};
#endif //TEMPERATUREIMPLICITBACKWARDDIFERENCE2
