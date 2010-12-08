#ifndef SOLIDMECHTEMPCOUPLEFALCON_H
#define SOLIDMECHTEMPCOUPLEFALCON_H

#include "SolidMechFalcon.h"

//libMesh includes
#include "tensor_value.h"


//Forward Declarations
class SolidMechTempCoupleFalcon;

template<>
InputParameters validParams<SolidMechTempCoupleFalcon>();

class SolidMechTempCoupleFalcon : public SolidMechFalcon
{
public:

  SolidMechTempCoupleFalcon(const std::string & name, InputParameters parameters);
  
  void recomputeCouplingConstants();
  
protected:
  unsigned int _temp_var;
  
  MaterialProperty<Real> & _thermal_strain;
  MaterialProperty<Real> & _alpha;

  Real _c4;
};
 

#endif //SOLIDMECHTEMPCOUPLEFALCON_H
