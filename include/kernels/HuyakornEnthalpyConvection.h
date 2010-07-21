#ifndef HUYAKORNENTHALPYCONVECTION
#define HUYAKORNENTHALPYCONVECTION

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class HuyakornEnthalpyConvection;

template<>
InputParameters validParams<HuyakornEnthalpyConvection>();

class HuyakornEnthalpyConvection : public Kernel
{
public:

  HuyakornEnthalpyConvection(std::string name, MooseSystem & moose_system, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();  

  
 
  MaterialProperty<Real> &_dTbydP_H;
  MaterialProperty<Real> &_Hs;
  MaterialProperty<Real> &_Hw;
  MaterialProperty<Real> &_thermal_conductivity;
  MaterialProperty<Real> &_darcy_params_s;
  MaterialProperty<Real> &_darcy_params_w;
  VariableGradient & _grad_p;
  
};
#endif //HUYAKORNENTHALPYCONVECTION
