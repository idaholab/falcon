#ifndef SOLIDMECHPOROCOUPLEX
#define SOLIDMECHPOROCOUPLEX

#include "SolidMechPoroCouple.h"


//Forward Declarations
class SolidMechPoroCoupleX;

template<>
InputParameters validParams<SolidMechPoroCoupleX>();

class SolidMechPoroCoupleX : public SolidMechPoroCouple
{
public:

  SolidMechPoroCoupleX(const std::string & name, MooseSystem & moose_system, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

};
#endif //SOLIDMECHPOROCOUPLEX 
