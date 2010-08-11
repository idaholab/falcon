#ifndef SOLIDMECHTEMPCOUPLEXFALCON
#define SOLIDMECHTEMPCOUPLEXFALCON

#include "SolidMechTempCoupleFalcon.h"


//Forward Declarations
class SolidMechTempCoupleXFalcon;

template<>
InputParameters validParams<SolidMechTempCoupleXFalcon>();

class SolidMechTempCoupleXFalcon : public SolidMechTempCoupleFalcon
{
public:

  SolidMechTempCoupleXFalcon(std::string name, MooseSystem & moose_system, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

};
#endif //SOLIDMECHTEMPCOUPLEXFALCON 
