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

  
 
  MaterialProperty<Real> &_lamda;
  VariableGradient & _grad_p;
};
#endif //HUYAKORNENTHALPYCONVECTION
