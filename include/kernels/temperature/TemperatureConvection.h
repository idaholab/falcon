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

  TemperatureConvection(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();  

  MaterialProperty<Real> &_specific_heat_water;
  MaterialProperty<RealGradient> &_darcy_flux_water;
  MaterialProperty<Real> &_density_water;
//  MooseArray<RealGradient> &_pore_velocity_water;
  
};
#endif //TEMPERATURECONVECTION
