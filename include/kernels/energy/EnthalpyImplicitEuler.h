
#include "ImplicitEuler.h"

#ifndef ENTHALPYIMPLICITEULER
#define ENTHALPYIMPLICITEULER

//Forward Declarations
class EnthalpyImplicitEuler;

template<>
InputParameters validParams<EnthalpyImplicitEuler>();

class EnthalpyImplicitEuler : public ImplicitEuler
{
public:

  EnthalpyImplicitEuler(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

//  VariableValue & _temperature;
//  VariableValue & _temperature_old;
//  VariableValue & _density;
//  VariableValue & _density_old;
  
  MaterialProperty<Real> & _temperature;
  MaterialProperty<Real> & _temperature_old;
  MaterialProperty<Real> & _density;
  MaterialProperty<Real> & _density_old;

  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _density_rock;
  
};
#endif //ENTHALPYIMPLICITEULER
