#ifndef DARCYMASSFLUXZSTEAM
#define DARCYMASSFLUXZSTEAM

#include "Kernel.h"

//Forward Declarations
class DarcyMassFluxZSteam;

template<>
InputParameters validParams<DarcyMassFluxZSteam>();

class DarcyMassFluxZSteam : public Kernel
{
public:

  DarcyMassFluxZSteam(std::string name, MooseSystem & moose_system, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpJacobian();

  MooseArray<Real> &_darcy_params_s;
  MooseArray<Real> &_gravity;
  MooseArray<Real> &_rho_s;
  
  MooseArray<RealVectorValue> &_gravity_vector;
};
#endif //DARCYMASSFLUXZSTEAM
