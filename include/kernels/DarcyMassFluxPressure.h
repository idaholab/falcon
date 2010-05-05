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
    
  virtual void subdomainSetup();

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  std::vector<Real> *_darcy_params;
};
#endif //DARCYMASSFLUXPRESSURE
