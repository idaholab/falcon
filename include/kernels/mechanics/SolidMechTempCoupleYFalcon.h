#ifndef SOLIDMECHTEMPCOUPLEYFALCON
#define SOLIDMECHTEMPCOUPLEYFALCON

#include "SolidMechTempCoupleFalcon.h"


//Forward Declarations
class SolidMechTempCoupleYFalcon;

template<>
InputParameters validParams<SolidMechTempCoupleYFalcon>();

class SolidMechTempCoupleYFalcon : public SolidMechTempCoupleFalcon
{
public:

  SolidMechTempCoupleYFalcon(const std::string & name, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  
};
 
#endif //SOLIDMECHTEMPCOUPLEYFALCON
