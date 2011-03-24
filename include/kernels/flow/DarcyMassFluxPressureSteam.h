#ifndef DARCYMASSFLUXPRESSURESTEAM
#define DARCYMASSFLUXPRESSURESTEAM

#include "Diffusion.h"
#include "Material.h"

//Forward Declarations
class DarcyMassFluxPressureSteam;

template<>
InputParameters validParams<DarcyMassFluxPressureSteam>();

class DarcyMassFluxPressureSteam : public Diffusion
{
public:

  DarcyMassFluxPressureSteam(const std::string & name, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  MaterialProperty<Real> &_tau_steam;
};
#endif //DARCYMASSFLUXPRESSURESTEAM
