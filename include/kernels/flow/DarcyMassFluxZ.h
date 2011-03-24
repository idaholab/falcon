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

  DarcyMassFluxZ(const std::string & name, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpJacobian();

  MaterialProperty<Real> &_tau_water;
  MaterialProperty<Real> &_gravity;
  MaterialProperty<Real> &_density_water;
  
  MaterialProperty<RealVectorValue> &_gravity_vector;
};
#endif //DARCYMASSFLUXZ
