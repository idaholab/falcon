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

  EnthalpyConvectionWater(std::string name, MooseSystem & moose_system, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();  

  VariableValue & _pressure;
  VariableGradient & _grad_p;
  MaterialProperty<RealGradient> &_darcy_flux_w;
  MaterialProperty<Real> &_rho_w;

  
};
#endif //ENTHALPYCONVECTIONWATER
