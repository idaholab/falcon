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

  MaterialProperty<Real> &_tau_steam;
  MaterialProperty<Real> &_gravity;
  MaterialProperty<Real> &_density_steam;
  
  MaterialProperty<RealVectorValue> &_gravity_vector;
};
#endif //DARCYMASSFLUXZSTEAM
