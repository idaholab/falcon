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

  SolidMechPoroCoupleX(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

};
#endif //SOLIDMECHPOROCOUPLEX
