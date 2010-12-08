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
 
  MaterialProperty<Real> &_Hw;
  MaterialProperty<Real> &_Hs;
  MaterialProperty<Real> &_darcy_params_s;
  MaterialProperty<Real> &_darcy_params_w;
  
};
#endif //ENTHALPYCONVECTION
