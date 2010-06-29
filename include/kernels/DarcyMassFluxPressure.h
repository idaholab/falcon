#ifndef DARCYMASSFLUXPRESSURE
#define DARCYMASSFLUXPRESSURE

#include "Diffusion.h"
#include "Material.h"

//Forward Declarations
class DarcyMassFluxPressure;

template<>
InputParameters validParams<DarcyMassFluxPressure>();

class DarcyMassFluxPressure : public Diffusion
{
public:

  DarcyMassFluxPressure(std::string name, MooseSystem & moose_system, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  MooseArray<Real> &_darcy_params_w;
};
#endif //DARCYMASSFLUXPRESSURE
