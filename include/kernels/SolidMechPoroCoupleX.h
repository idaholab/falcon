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

  SolidMechPoroCoupleX(std::string name,
                       InputParameters parameters,
                       std::string var_name,
                       std::vector<std::string> coupled_to,
                       std::vector<std::string> coupled_as);
  
protected:
  virtual Real computeQpResidual();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

};
#endif //SOLIDMECHPOROCOUPLEX 
