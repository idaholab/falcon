#ifndef VELOCITY_H
#define VELOCITY_H

#include "Kernel.h"
#include "Material.h"


//ForwardDeclarations
class Velocity;

template<>
InputParameters validParams<Velocity>();

class Velocity : public Kernel
{
public:

  Velocity(std::string name,
                  InputParameters parameters,
                  std::string var_name,
                  std::vector<std::string> coupled_to=std::vector<std::string>(0),
           std::vector<std::string> coupled_as=std::vector<std::string>(0));
  
protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  unsigned int _p_var;
  std::vector<Real> & _p;

  unsigned int _momentum_var;
  std::vector<Real> & _momentum;

  Real _acceleration;
};
 
#endif
