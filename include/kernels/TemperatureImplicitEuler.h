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

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  MaterialProperty<Real> & _water_specific_heat;
  MaterialProperty<Real> & _rock_specific_heat;
  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _rho_w;
  MaterialProperty<Real> & _rho_r;
  
};
#endif //TEMPERATUREIMPLICITEULER
