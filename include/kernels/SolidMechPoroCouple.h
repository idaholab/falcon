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
  MooseArray<Real> * _biot_coeff;
  MooseArray<RealGradient> & _grad_pressure;
  MooseArray<Real> & _pressure_val;

};
 

#endif //SOLIDMECHPOROCOUPLE_H
