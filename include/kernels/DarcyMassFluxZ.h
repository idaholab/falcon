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

  DarcyMassFluxZ(const std::string & name, MooseSystem & moose_system, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpJacobian();

  MaterialProperty<Real> &_darcy_params_w;
  MaterialProperty<Real> &_gravity;
  MaterialProperty<Real> &_rho_w;
  
  MaterialProperty<RealVectorValue> &_gravity_vector;
};
#endif //DARCYMASSFLUXZ
