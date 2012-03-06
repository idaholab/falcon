/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

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

  SolidMechTempCoupleXFalcon(const std::string & name, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

};
#endif //SOLIDMECHTEMPCOUPLEXFALCON 
