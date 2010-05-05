#ifndef STRAINCOMPUTE_H
#define STRAINCOMPUTE_H

#include "Kernel.h"
#include "Material.h"


//ForwardDeclarations
class StrainCompute;

template<>
InputParameters validParams<StrainCompute>();

class StrainCompute : public Kernel
{
public:

  StrainCompute(std::string name, MooseSystem & moose_system, InputParameters parameters);
  
  virtual void subdomainSetup();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  int _component;
  std::vector<RealVectorValue> *_stress_normal_vector;
  std::vector<RealVectorValue> *_stress_shear_vector;
};
 
#endif
