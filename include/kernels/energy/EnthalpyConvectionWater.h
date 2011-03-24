#ifndef ENTHALPYCONVECTIONWATER
#define ENTHALPYCONVECTIONWATER

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class EnthalpyConvectionWater;

template<>
InputParameters validParams<EnthalpyConvectionWater>();

class EnthalpyConvectionWater : public Kernel
{
public:

  EnthalpyConvectionWater(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();  

  MaterialProperty<RealGradient> &_darcy_flux_water;
  MaterialProperty<RealGradient> &_Genthalpy_saturated_water;
  MaterialProperty<Real> &_density_water;

  
};
#endif //ENTHALPYCONVECTIONWATER
