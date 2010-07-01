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

  DarcyImplicitEuler(std::string name, MooseSystem & moose_system, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  MaterialProperty<Real> & _c_f;
  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _rho_w;
  
};
#endif //DARCYIMPLICITEULER
