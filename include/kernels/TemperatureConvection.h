#ifndef TEMPERATURECONVECTION
#define TEMPERATURECONVECTION

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class TemperatureConvection;

template<>
InputParameters validParams<TemperatureConvection>();

class TemperatureConvection : public Kernel
{
public:

  TemperatureConvection(std::string name, MooseSystem & moose_system, InputParameters parameters);

   virtual void subdomainSetup();
  
protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();  

  std::vector<Real> *_water_specific_heat;
  std::vector<RealGradient> *_darcy_flux;
  std::vector<Real> *_rho_w;
//  std::vector<RealGradient> *_pore_velocity;
  
};
#endif //TEMPERATURECONVECTION
