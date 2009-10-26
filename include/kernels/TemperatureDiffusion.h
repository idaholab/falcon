#ifndef TEMPERATUREDIFFUSION
#define TEMPERATUREDIFFUSION

#include "Diffusion.h"
#include "Material.h"


class TemperatureDiffusion : public Diffusion
{
public:

  TemperatureDiffusion(std::string name,
                Parameters parameters,
                std::string var_name,
                std::vector<std::string> coupled_to,
                std::vector<std::string> coupled_as);
    
  virtual void subdomainSetup();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  std::vector<Real> *_thermal_conductivity;
  
};
#endif //TEMPERATUREDIFFUSION
