#ifndef SOLIDMECHPOROCOUPLEZ
#define SOLIDMECHPOROCOUPLEZ

#include "SolidMechPoroCouple.h"


//Forward Declarations
class SolidMechPoroCoupleZ;

template<>
InputParameters validParams<SolidMechPoroCoupleZ>();

class SolidMechPoroCoupleZ : public SolidMechPoroCouple
{
public:

  SolidMechPoroCoupleZ(const std::string & name, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  
};
 
#endif //SOLIDMECHPOROCOUPLEZ
