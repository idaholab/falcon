#ifndef STRESSCOMPUTE_H
#define STRESSCOMPUTE_H

#include "Kernel.h"
#include "Material.h"


//ForwardDeclarations
class StressCompute;

template<>
InputParameters validParams<StressCompute>();

class StressCompute : public Kernel
{
public:

  StressCompute(std::string name, MooseSystem & moose_system, InputParameters parameters);
  
  virtual void subdomainSetup();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  int _component;
  MooseArray<RealVectorValue> *_stress_normal_vector;
  MooseArray<RealVectorValue> *_stress_shear_vector;

private:
};
 
#endif
