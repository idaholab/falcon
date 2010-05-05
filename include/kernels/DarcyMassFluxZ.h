#ifndef DARCYMASSFLUXZ
#define DARCYMASSFLUXZ

#include "Kernel.h"

//Forward Declarations
class DarcyMassFluxZ;

template<>
InputParameters validParams<DarcyMassFluxZ>();

class DarcyMassFluxZ : public Kernel
{
public:

  DarcyMassFluxZ(std::string name, MooseSystem & moose_system, InputParameters parameters);
    
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
