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

  DarcyMassFluxPressureSteam(const std::string & name, MooseSystem & moose_system, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  MaterialProperty<Real> &_darcy_params_s;
};
#endif //DARCYMASSFLUXPRESSURESTEAM
