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

  SolidMechPoroCoupleY(std::string name,
                       InputParameters parameters,
                       std::string var_name,
                       std::vector<std::string> coupled_to,
                       std::vector<std::string> coupled_as);
  
protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  
};
 
#endif //SOLIDMECHPOROCOUPLEY
