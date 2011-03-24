#include "ImplicitEuler.h"

#ifndef COUPLEDDARCYIMPLICITEULER
#define COUPLEDDARCYIMPLICITEULER

//Forward Declarations
class CoupledDarcyImplicitEuler;

template<>
InputParameters validParams<CoupledDarcyImplicitEuler>();

class CoupledDarcyImplicitEuler : public ImplicitEuler
{
public:

  CoupledDarcyImplicitEuler(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

//   VariableValue & _density;
//   VariableValue & _density_old;  
  MaterialProperty<Real> & _density;
  MaterialProperty<Real> & _density_old;

  VariableValue & _enthalpy;
  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _S_water;
  MaterialProperty<Real> & _S_steam;
  MaterialProperty<Real> & _enthalpy_saturated_water;
  MaterialProperty<Real> & _enthalpy_saturated_steam;

  Real E3;
  Real E6;
  
  Real a2;
  Real a5;
  
  Real b2;
  Real b3;
  Real b4;
  Real b5;
};
#endif //COUPLEDDARCYIMPLICITEULER
