#ifndef PRESCRIBEDEXPANSIONX
#define PRESCRIBEDEXPANSIONX

#include "SolidMechTempCoupleFalcon.h"


//Forward Declarations
class PrescribedExpansion;

template<>
InputParameters validParams<PrescribedExpansion>();

class PrescribedExpansion : public SolidMechTempCoupleFalcon
{
public:

  PrescribedExpansion(const std::string & name, MooseSystem & moose_system, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();
  
private:
  int _component;
  Real _percent;
};
#endif //PRESCRIBEDEXPANSIONX 
