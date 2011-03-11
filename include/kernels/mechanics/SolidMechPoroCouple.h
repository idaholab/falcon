#ifndef SOLIDMECHPOROCOUPLE_H
#define SOLIDMECHPOROCOUPLE_H

#include "SolidMechFalcon.h"

//libMesh includes
#include "tensor_value.h"


//Forward Declarations
class SolidMechPoroCouple;

template<>
InputParameters validParams<SolidMechPoroCouple>();

class SolidMechPoroCouple : public SolidMechFalcon
{
public:

  SolidMechPoroCouple(const std::string & name, InputParameters parameters);
  
  void recomputeCouplingConstants();
  
protected:
  unsigned int _pressure_var;
  MaterialProperty<Real> & _biot_coeff;
  VariableGradient & _grad_pressure;
  VariableValue  & _pressure_val;

};
 

#endif //SOLIDMECHPOROCOUPLE_H
