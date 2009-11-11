#ifndef DARCYVELOCITY_H
#define DARCYVELOCITY_H

#include "Kernel.h"
#include "Material.h"


//ForwardDeclarations
class DarcyVelocity;

template<>
InputParameters valid_params<DarcyVelocity>();

class DarcyVelocity : public Kernel
{
public:

  DarcyVelocity(std::string name,
                InputParameters parameters,
                std::string var_name,
                std::vector<std::string> coupled_to=std::vector<std::string>(0),
                std::vector<std::string> coupled_as=std::vector<std::string>(0));
  
  virtual void subdomainSetup();


protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  int _component;
  std::vector<RealGradient> *_darcy_velocity;

private:
};
 
#endif
