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
  SolidMechSwellingGas(std::string name, MooseSystem & moose_system, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();
  
private:
  MooseArray<Real> & _temp;

  Real _burnup;
  int _component;

  MooseArray<Real> & _density;
};
#endif //SOLIDMECHSWELLINGGAS 
