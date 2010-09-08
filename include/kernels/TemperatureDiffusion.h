#ifndef TEMPERATUREDIFFUSION
#define TEMPERATUREDIFFUSION

#include "Diffusion.h"
#include "Material.h"

class TemperatureDiffusion;

template<>
InputParameters validParams<TemperatureDiffusion>();

class TemperatureDiffusion : public Diffusion
{
public:

  TemperatureDiffusion(const std::string & name, MooseSystem & moose_system, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  MaterialProperty<Real> &_thermal_conductivity;
  
};
#endif //TEMPERATUREDIFFUSION
