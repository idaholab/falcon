#ifndef ENTHALPYDIFFUSION
#define ENTHALPYDIFFUSION

#include "Diffusion.h"
#include "Material.h"

class EnthalpyDiffusion;

template<>
InputParameters validParams<EnthalpyDiffusion>();

class EnthalpyDiffusion : public Diffusion
{
public:

  EnthalpyDiffusion(const std::string & name, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  

  VariableGradient & _grad_T;
  VariableValue & _dTdH_P;
   VariableValue & _dTdP_H;
   unsigned int _p_var;
  MaterialProperty<Real> &_thermal_conductivity;

  
};
#endif //ENTHALPYDIFFUSION
