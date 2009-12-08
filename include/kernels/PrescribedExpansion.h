#ifndef PRESCRIBEDEXPANSIONX
#define PRESCRIBEDEXPANSIONX

#include "SolidMechTempCouple.h"


//Forward Declarations
class PrescribedExpansion;

template<>
InputParameters validParams<PrescribedExpansion>();

class PrescribedExpansion : public SolidMechTempCouple
{
public:

  PrescribedExpansion(std::string name,
                      InputParameters parameters,
                      std::string var_name,
                      std::vector<std::string> coupled_to,
                      std::vector<std::string> coupled_as);
  
protected:
  virtual Real computeQpResidual();
  
private:
  int _component;
  Real _percent;
};
#endif //PRESCRIBEDEXPANSIONX 
