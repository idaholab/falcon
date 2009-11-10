#ifndef SOLIDMECHSWELLINGGAS
#define SOLIDMECHSWELLINGGAS

#include "Kernel.h"
#include "Material.h"


//Forward Declarations
class SolidMechSwellingGas;

template<>
InputParameters valid_params<SolidMechSwellingGas>();

class SolidMechSwellingGas : public Kernel
{
public:
  SolidMechSwellingGas(std::string name,
                       InputParameters parameters,
                       std::string var_name,
                       std::vector<std::string> coupled_to,
                       std::vector<std::string> coupled_as);
  
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
