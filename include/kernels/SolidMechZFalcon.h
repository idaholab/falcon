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

  SolidMechZFalcon(std::string name, MooseSystem & moose_system, InputParameters parameters);
  
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
