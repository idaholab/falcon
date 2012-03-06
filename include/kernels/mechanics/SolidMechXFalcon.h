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

#ifndef SOLIDMECHXFALCON
#define SOLIDMECHXFALCON

#include "SolidMechFalcon.h"


//Forward Declarations
class SolidMechXFalcon;

template<>
InputParameters validParams<SolidMechXFalcon>();

class SolidMechXFalcon : public SolidMechFalcon
{
public:

  SolidMechXFalcon(const std::string & name, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  
private:
  unsigned int _y_var;
  VariableValue  & _y;
  VariableGradient & _grad_y;

  unsigned int _z_var;
  VariableValue  & _z;
  VariableGradient & _grad_z;
};
#endif //SOLIDMECHXFALCON 
