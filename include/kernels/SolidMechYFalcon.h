#ifndef SOLIDMECHYFALCON
#define SOLIDMECHYFALCON

#include "SolidMechFalcon.h"


//Forward Declarations
class SolidMechYFalcon;

template<>
InputParameters validParams<SolidMechYFalcon>();

class SolidMechYFalcon : public SolidMechFalcon
{
public:

  SolidMechYFalcon(std::string name, MooseSystem & moose_system, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  
private:
  unsigned int _x_var;
  VariableValue  & _x;
  VariableGradient & _grad_x;
  
  unsigned int _z_var;
  VariableValue  & _z;
  VariableGradient & _grad_z;
};
#endif //SOLIDMECHYFALCON 
