#ifndef SOLIDMECHSWELLINGSOLID
#define SOLIDMECHSWELLINGSOLID

#include "SolidMechTempCoupleFalcon.h"

//Forward Declarations
class SolidMechSwellingSolid;

template<>
InputParameters validParams<SolidMechSwellingSolid>();

class SolidMechSwellingSolid : public SolidMechTempCoupleFalcon
{
public:
  SolidMechSwellingSolid(const std::string & name, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();
  
private:
  MaterialProperty<Real> & _solid_swelling_strain;

  Real _burnup;
  int _component;
};
#endif //SOLIDMECHSWELLINGSOLID 
