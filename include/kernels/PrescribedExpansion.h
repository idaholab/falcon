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

  PrescribedExpansion(std::string name, MooseSystem & moose_system, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();
  
private:
  int _component;
  Real _percent;
};
#endif //PRESCRIBEDEXPANSIONX 
