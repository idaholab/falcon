#ifndef WATERMASSFLUXELEVATION
#define WATERMASSFLUXELEVATION

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class WaterMassFluxElevation;

template<>
InputParameters validParams<WaterMassFluxElevation>();

class WaterMassFluxElevation : public Kernel
{
public:

  WaterMassFluxElevation(const std::string & name, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();


  VariableValue  & _density_water;

  
  MaterialProperty<Real> & _tau_water;
  MaterialProperty<Real> & _gravity;
  MaterialProperty<RealVectorValue> & _gravity_vector;

};
#endif //WATERMASSFLUXELEVATION
