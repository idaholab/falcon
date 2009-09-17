#ifndef SOLIDMECHSWELLINGSOLID
#define SOLIDMECHSWELLINGSOLID

#include "SolidMechTempCouple.h"
#include "Material.h"


//Forward Declarations
class SolidMechSwellingSolid;

template<>
Parameters valid_params<SolidMechSwellingSolid>();

class SolidMechSwellingSolid : public SolidMechTempCouple
{
public:
  SolidMechSwellingSolid(std::string name,
                         Parameters parameters,
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
