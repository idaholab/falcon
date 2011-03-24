#ifndef ENTHALPYCONVECTIONZWATER
#define ENTHALPYCONVECTIONZWATER

#include "Kernel.h"

//Forward Declarations
class EnthalpyConvectionZWater;

template<>
InputParameters validParams<EnthalpyConvectionZWater>();

class EnthalpyConvectionZWater : public Kernel
{
public:

  EnthalpyConvectionZWater(const std::string & name, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpJacobian();

  MaterialProperty<Real> &_tau_water;
  MaterialProperty<Real> &_gravity;
  MaterialProperty<Real> &_density_water;
  MaterialProperty<Real> &_enthalpy_saturated_water;
  
  MaterialProperty<RealVectorValue> &_gravity_vector;
};
#endif //ENTHALPYCONVECTIONZWATER
