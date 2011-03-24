#include "ImplicitEuler.h"

#ifndef COUPLEDMASSFLUXIMPLICITEULER
#define COUPLEDMASSFLUXIMPLICITEULER

//Forward Declarations
class CoupledMassFluxImplicitEuler;

template<>
InputParameters validParams<CoupledMassFluxImplicitEuler>();

class CoupledMassFluxImplicitEuler : public ImplicitEuler
{
public:

  CoupledMassFluxImplicitEuler(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

//  VariableValue & _compressibility;
//  VariableValue & _density;

  MaterialProperty<Real> & _density_water;
  MaterialProperty<Real> & _compressibility;


  MaterialProperty<Real> & _porosity;

};
#endif //COUPLEDMASSFLUXIMPLICITEULER
