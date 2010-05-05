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
  
  virtual void subdomainSetup();
  
protected:
  virtual Real computeQpResidual();
  
private:
  std::vector<Real> & _temp;

  Real _burnup;
  int _component;

  std::vector<Real> * _density;
};
#endif //SOLIDMECHSWELLINGGAS 
