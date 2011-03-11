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

  HuyakornEnthalpyConvection(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();  

  
 
  VariableGradient & _grad_p;
  MaterialProperty<Real> &_lamda;
};
#endif //HUYAKORNENTHALPYCONVECTION
