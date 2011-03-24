#ifndef ENTHALPYCONVECTION
#define ENTHALPYCONVECTION

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class EnthalpyConvection;

template<>
InputParameters validParams<EnthalpyConvection>();

class EnthalpyConvection : public Kernel
{
public:

  EnthalpyConvection(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();  

  
  VariableGradient & _grad_p;
 
  MaterialProperty<Real> &_enthalpy_saturated_water;
  MaterialProperty<Real> &_enthalpy_saturated_steam;
  MaterialProperty<Real> &_tau_steam;
  MaterialProperty<Real> &_tau_water;
  
};
#endif //ENTHALPYCONVECTION
