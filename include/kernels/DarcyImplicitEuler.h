#include "ImplicitEuler.h"

#ifndef DARCYIMPLICITEULER
#define DARCYIMPLICITEULER



class DarcyImplicitEuler : public ImplicitEuler
{
public:

  DarcyImplicitEuler(std::string name,
                     Parameters parameters,
                     std::string var_name,
                     std::vector<std::string> coupled_to,
                     std::vector<std::string> coupled_as);

  virtual void subdomainSetup();

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  std::vector<Real> * _c_f;
  std::vector<Real> * _porosity;
  std::vector<Real> * _rho_w;
  
};
#endif //DARCYIMPLICITEULER
