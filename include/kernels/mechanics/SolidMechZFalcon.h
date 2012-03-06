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

#ifndef SOLIDMECHZFALCON
#define SOLIDMECHZFALCON

#include "SolidMechFalcon.h"

//Forward Declarations
class SolidMechZFalcon;

template<>
InputParameters validParams<SolidMechZFalcon>();

class SolidMechZFalcon : public SolidMechFalcon
{
public:

  SolidMechZFalcon(const std::string & name, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpJacobian();
  
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  
private:
  unsigned int _x_var;
  VariableValue  & _x;
  VariableGradient & _grad_x;
  
  unsigned int _y_var;
  VariableValue  & _y;
  VariableGradient & _grad_y;
};
 
#endif //SOLIDMECHZFALCON
