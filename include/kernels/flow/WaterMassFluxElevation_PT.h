#ifndef WATERMASSFLUXELEVATION_PT
#define WATERMASSFLUXELEVATION_PT

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class WaterMassFluxElevation_PT;

template<>
InputParameters validParams<WaterMassFluxElevation_PT>();

class WaterMassFluxElevation_PT : public Kernel
{
public:

  WaterMassFluxElevation_PT(const std::string & name, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();


  VariableValue  & _density_water;

  
  MaterialProperty<Real> & _tau_water;
  MaterialProperty<Real> & _gravity;
  MaterialProperty<RealVectorValue> & _gravity_vector;

};
#endif //WATERMASSFLUXELEVATION
