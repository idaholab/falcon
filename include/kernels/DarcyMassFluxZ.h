#ifndef DARCYMASSFLUXZ
#define DARCYMASSFLUXZ

#include "Material.h"


class DarcyMassFluxZ : public Kernel
{
public:

  DarcyMassFluxZ(std::string name,
                 Parameters parameters,
                 std::string var_name,
                 std::vector<std::string> coupled_to,
                 std::vector<std::string> coupled_as);
    
  virtual void subdomainSetup();

protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpJacobian();

  std::vector<Real> *_darcy_params;
  std::vector<Real> *_gravity;
  std::vector<Real> *_rho_w;
  
  std::vector<RealVectorValue> *_gravity_vector;
};
#endif //DARCYMASSFLUXZ
