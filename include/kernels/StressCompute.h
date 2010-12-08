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

  StressCompute(const std::string & name, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  int _component;
  MaterialProperty<RealVectorValue> &_stress_normal_vector;
  MaterialProperty<RealVectorValue> &_stress_shear_vector;

private:
};
 
#endif
