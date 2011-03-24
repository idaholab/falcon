#include "ImplicitEuler.h"

#ifndef DARCYIMPLICITEULER
#define DARCYIMPLICITEULER

//Forward Declarations
class DarcyImplicitEuler;

template<>
InputParameters validParams<DarcyImplicitEuler>();

class DarcyImplicitEuler : public ImplicitEuler
{
public:

  DarcyImplicitEuler(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  MaterialProperty<Real> & _compressibility;
  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _density_water;
  
};
#endif //DARCYIMPLICITEULER
