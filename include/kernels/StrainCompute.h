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

  StrainCompute(std::string name,
                InputParameters parameters,
                std::string var_name,
                std::vector<std::string> coupled_to=std::vector<std::string>(0),
                std::vector<std::string> coupled_as=std::vector<std::string>(0));
  
  virtual void subdomainSetup();


protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  int _component;
  std::vector<RealVectorValue> *_stress_normal_vector;
  std::vector<RealVectorValue> *_stress_shear_vector;

private:
};
 
#endif
