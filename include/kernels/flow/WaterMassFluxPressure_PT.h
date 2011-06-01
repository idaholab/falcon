#ifndef WATERMASSFLUXPRESSURE_PT
#define WATERMASSFLUXPRESSURE_PT

#include "Diffusion.h"
#include "Material.h"

//Forward Declarations
class WaterMassFluxPressure_PT;

template<>
InputParameters validParams<WaterMassFluxPressure_PT>();

class WaterMassFluxPressure_PT : public Diffusion
{
public:

  WaterMassFluxPressure_PT(const std::string & name, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  MaterialProperty<Real> & _tau_water;
};
#endif //WATERMASSFLUXPRESSURE
