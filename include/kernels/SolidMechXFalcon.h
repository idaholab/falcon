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

  SolidMechXFalcon(const std::string & name, MooseSystem & moose_system, InputParameters parameters);
  
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
