#ifndef TEMPERATURECONVECTIONPRESSURE
#define TEMPERATURECONVECTIONPRESSURE

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class TemperatureConvectionPressure;

template<>
InputParameters validParams<TemperatureConvectionPressure>();

class TemperatureConvectionPressure : public Kernel
{
public:

  TemperatureConvectionPressure(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();  

  MaterialProperty<Real> & _specific_heat_water;
  MaterialProperty<RealGradient> & _darcy_mass_flux_water_pressure;
  MaterialProperty<Real> & _density_water;
//  MooseArray<RealGradient> &_pore_velocity_water;
  
};
#endif //TEMPERATURECONVECTIONPRESSURE
