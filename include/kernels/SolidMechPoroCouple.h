#ifndef SOLIDMECHPOROCOUPLE_H
#define SOLIDMECHPOROCOUPLE_H

#include "SolidMech.h"

//libMesh includes
#include "tensor_value.h"


//Forward Declarations
class SolidMechPoroCouple;

template<>
InputParameters validParams<SolidMechPoroCouple>();

class SolidMechPoroCouple : public SolidMech
{
public:

  SolidMechPoroCouple(std::string name, MooseSystem & moose_system, InputParameters parameters);
  
  virtual void subdomainSetup();
  
  void recomputeCouplingConstants();
  
protected:
  unsigned int _pressure_var;
  std::vector<Real> * _biot_coeff;
  std::vector<RealGradient> & _grad_pressure;
  std::vector<Real> & _pressure_val;

};
 

#endif //SOLIDMECHPOROCOUPLE_H
