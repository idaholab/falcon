#ifndef SOLIDMECHSWELLINGSOLID
#define SOLIDMECHSWELLINGSOLID

#include "SolidMechTempCouple.h"
#include "Material.h"


//Forward Declarations
class SolidMechSwellingSolid;

template<>
InputParameters validParams<SolidMechSwellingSolid>();

class SolidMechSwellingSolid : public SolidMechTempCouple
{
public:
  SolidMechSwellingSolid(std::string name,
                         InputParameters parameters,
                         std::string var_name,
                         std::vector<std::string> coupled_to,
                         std::vector<std::string> coupled_as);
  
  virtual void subdomainSetup();
  
protected:
  virtual Real computeQpResidual();
  
private:
  std::vector<Real> * _solid_swelling_strain;

  Real _burnup;
  int _component;
};
#endif //SOLIDMECHSWELLINGSOLID 
