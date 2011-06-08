#ifndef GRAVITY_H
#define GRAVITY_H

#include "Kernel.h"
#include "Material.h"

//libMesh includes
#include "tensor_value.h"
#include "vector_value.h"


//Forward Declarations
class Gravity;

template<>
InputParameters validParams<Gravity>();

class Gravity : public Kernel
{
public:

  Gravity(const std::string & name, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpJacobian();
  int _component;
  MaterialProperty<Real> & _density;
  MaterialProperty<Real> & _gravity;
  MaterialProperty<RealVectorValue> & _gravity_vector;

};
 

#endif //GRAVITY_H
