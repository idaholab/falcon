#ifndef SOLIDMECHSWELLINGGAS
#define SOLIDMECHSWELLINGGAS

#include "Kernel.h"
#include "Material.h"


//Forward Declarations
class SolidMechSwellingGas;

template<>
InputParameters validParams<SolidMechSwellingGas>();

class SolidMechSwellingGas : public Kernel
{
public:
  SolidMechSwellingGas(const std::string & name, MooseSystem & moose_system, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();
  
private:
  VariableValue & _temp;

  Real _burnup;
  int _component;

  MaterialProperty<Real> & _density;
};
#endif //SOLIDMECHSWELLINGGAS 
