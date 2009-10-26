#ifndef TEMPERATURECONVECTION
#define TEMPERATURECONVECTION

#include "Kernel.h"
#include "Material.h"


class TemperatureConvection : public Kernel
{
public:

  TemperatureConvection(std::string name,
                        Parameters parameters,
                        std::string var_name,
                        std::vector<std::string> coupled_to,
                        std::vector<std::string> coupled_as);

   virtual void subdomainSetup();
  
protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();  

  std::vector<Real> *_water_specific_heat;
  std::vector<RealGradient> *_darcy_velocity;
  
};
#endif //TEMPERATURECONVECTION
