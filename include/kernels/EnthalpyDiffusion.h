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

  VariableValue & _pressure;
  VariableGradient & _grad_p;
  MaterialProperty<Real> &_dTbydP_H;
  MaterialProperty<Real> &_dTbydH_P;
  MaterialProperty<Real> &_thermal_conductivity;

//  VariableGradient & _grad_T;
  
};
#endif //ENTHALPYDIFFUSION
