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

  EnthalpyDiffusion(std::string name, MooseSystem & moose_system, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  MooseArray<Real> &_thermal_conductivity;
  MooseArray<Real> &_dTbydP_H;
  MooseArray<Real> &_dTbydH_P;
  MooseArray<RealGradient> & _grad_p;
  MooseArray<RealGradient> & _grad_T;
  
};
#endif //ENTHALPYDIFFUSION
