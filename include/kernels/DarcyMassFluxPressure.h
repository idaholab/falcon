#ifndef DARCYMASSFLUXPRESSURE
#define DARCYMASSFLUXPRESSURE

#include "Diffusion.h"
#include "Material.h"


class DarcyMassFluxPressure : public Diffusion
{
public:

  DarcyMassFluxPressure(std::string name,
                InputParameters parameters,
                std::string var_name,
                std::vector<std::string> coupled_to,
                std::vector<std::string> coupled_as);
    
  virtual void subdomainSetup();

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  std::vector<Real> *_darcy_params;
};
#endif //DARCYMASSFLUXPRESSURE
