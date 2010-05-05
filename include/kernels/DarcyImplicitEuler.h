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

  std::vector<Real> * _c_f;
  std::vector<Real> * _porosity;
  std::vector<Real> * _rho_w;
  
};
#endif //DARCYIMPLICITEULER
