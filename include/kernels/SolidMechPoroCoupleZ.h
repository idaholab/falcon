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

  SolidMechPoroCoupleZ(std::string name, MooseSystem & moose_system, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  
};
 
#endif //SOLIDMECHPOROCOUPLEZ
