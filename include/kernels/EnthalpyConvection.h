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

  EnthalpyConvection(std::string name, MooseSystem & moose_system, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();  

  
 
  MaterialProperty<Real> &_Hs;
  MaterialProperty<Real> &_Hw;
  MaterialProperty<Real> &_darcy_params_s;
  MaterialProperty<Real> &_darcy_params_w;
  VariableGradient & _grad_p;
  
};
#endif //ENTHALPYCONVECTION
