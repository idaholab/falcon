#ifndef SOLIDMECHSWELLINGSOLID
#define SOLIDMECHSWELLINGSOLID

#include "SolidMechTempCouple.h"

//Forward Declarations
class SolidMechSwellingSolid;

template<>
InputParameters validParams<SolidMechSwellingSolid>();

class SolidMechSwellingSolid : public SolidMechTempCouple
{
public:
  SolidMechSwellingSolid(std::string name, MooseSystem & moose_system, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();
  
private:
  MooseArray<Real> & _solid_swelling_strain;

  Real _burnup;
  int _component;
};
#endif //SOLIDMECHSWELLINGSOLID 
