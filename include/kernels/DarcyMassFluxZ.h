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
    
protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpJacobian();

  MooseArray<Real> &_darcy_params_w;
  MooseArray<Real> &_gravity;
  MooseArray<Real> &_rho_w;
  
  MooseArray<RealVectorValue> &_gravity_vector;
};
#endif //DARCYMASSFLUXZ
