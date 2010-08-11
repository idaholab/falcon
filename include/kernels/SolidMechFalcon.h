#ifndef SOLIDMECHFALCON_H
#define SOLIDMECHFALCON_H

#include "Kernel.h"
#include "Material.h"

//libMesh includes
#include "tensor_value.h"
#include "vector_value.h"


//Forward Declarations
class SolidMechFalcon;

template<>
InputParameters validParams<SolidMechFalcon>();

class SolidMechFalcon : public Kernel
{
public:

  SolidMechFalcon(std::string name, MooseSystem & moose_system, InputParameters parameters);
  
  void recomputeConstants();
  
protected:
  Real _E;
  Real _nu;
  Real _c1;
  Real _c2;
  Real _c3;

  TensorValue<Number> _B11;
  TensorValue<Number> _B12;
  TensorValue<Number> _B13;
  TensorValue<Number> _B21;
  TensorValue<Number> _B22;
  TensorValue<Number> _B23;
  TensorValue<Number> _B31;
  TensorValue<Number> _B32;
  TensorValue<Number> _B33;

  VectorValue<Number> _stress;
  TensorValue<Number> _strain;

  Real _density;

  MaterialProperty<Real> & _E_prop;
  MaterialProperty<Real> & _nu_prop;
  MaterialProperty<RealVectorValue> & _stress_normal_vector;
  MaterialProperty<RealVectorValue> & _stress_shear_vector;
};
 

#endif //SOLIDMECHFALCON_H
