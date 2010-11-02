
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

  EnthalpyImplicitEuler(const std::string & name, MooseSystem & moose_system, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

//  VariableValue & _temperature;
//  VariableValue & _temperature_old;
//  VariableValue & _rho;
//  VariableValue & _rho_old;
  
  MaterialProperty<Real> & _temperature;
  MaterialProperty<Real> & _temperature_old;
  MaterialProperty<Real> & _rho;
  MaterialProperty<Real> & _rho_old;

  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _rho_r;
  
};
#endif //ENTHALPYIMPLICITEULER
