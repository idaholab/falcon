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

  CoupledDarcyImplicitEuler(const std::string & name, MooseSystem & moose_system, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  VariableValue & _enthalpy;
//   VariableValue & _rho;
//   VariableValue & _rho_old;  
  MaterialProperty<Real> & _rho;
  MaterialProperty<Real> & _rho_old;
  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _sat_w;
  MaterialProperty<Real> & _sat_s;
  MaterialProperty<Real> & _Hw;
  MaterialProperty<Real> & _Hs;

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
