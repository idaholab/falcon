#ifndef SOLIDMECHPOROCOUPLEY
#define SOLIDMECHPOROCOUPLEY

#include "SolidMechPoroCouple.h"


//Forward Declarations
class SolidMechPoroCoupleY;

template<>
InputParameters validParams<SolidMechPoroCoupleY>();

class SolidMechPoroCoupleY : public SolidMechPoroCouple
{
public:

  SolidMechPoroCoupleY(const std::string & name, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  
};
 
#endif //SOLIDMECHPOROCOUPLEY
