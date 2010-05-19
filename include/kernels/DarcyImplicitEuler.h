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

  virtual void subdomainSetup();

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  MooseArray<Real> * _c_f;
  MooseArray<Real> * _porosity;
  MooseArray<Real> * _rho_w;
  
};
#endif //DARCYIMPLICITEULER
