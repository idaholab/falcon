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

  
  MaterialProperty<RealGradient> &_darcy_flux_s;
  MaterialProperty<RealGradient> &_darcy_flux_w;
  MaterialProperty<RealGradient> &_GHs;
  MaterialProperty<RealGradient> &_GHw;
  MaterialProperty<Real> &_rho_s;
  MaterialProperty<Real> &_rho_w;
  
  MaterialProperty<Real> &_Hs;
  MaterialProperty<Real> &_Hw;
  MaterialProperty<Real> &_darcy_params_s;
  MaterialProperty<Real> &_darcy_params_w;
  VariableGradient & _grad_p;
  
};
#endif //ENTHALPYCONVECTION
