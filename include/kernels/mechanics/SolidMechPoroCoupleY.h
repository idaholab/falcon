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
