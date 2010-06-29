
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

  EnthalpyImplicitEuler(std::string name, MooseSystem & moose_system, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  MooseArray<Real> & _temperature;
  MooseArray<Real> & _temperature_old;
  MooseArray<Real> & _porosity;
  MooseArray<Real> & _rho_mix;
  MooseArray<Real> & _rho_r;
  
};
#endif //ENTHALPYIMPLICITEULER
