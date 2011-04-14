#ifndef WATERMASSFLUXPRESSURE
#define WATERMASSFLUXPRESSURE

#include "Diffusion.h"
#include "Material.h"

//Forward Declarations
class WaterMassFluxPressure;

template<>
InputParameters validParams<WaterMassFluxPressure>();

class WaterMassFluxPressure : public Diffusion
{
public:

  WaterMassFluxPressure(const std::string & name, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  MaterialProperty<Real> & _tau_water;
};
#endif //WATERMASSFLUXPRESSURE
