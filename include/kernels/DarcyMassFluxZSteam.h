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

  DarcyMassFluxZSteam(const std::string & name, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpJacobian();

  MaterialProperty<Real> &_darcy_params_s;
  MaterialProperty<Real> &_gravity;
  MaterialProperty<Real> &_rho_s;
  
  MaterialProperty<RealVectorValue> &_gravity_vector;
};
#endif //DARCYMASSFLUXZSTEAM
