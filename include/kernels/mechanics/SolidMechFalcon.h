/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

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

  SolidMechFalcon(const std::string & name, InputParameters parameters);
  
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
//  MaterialProperty<Real> & _damage_coeff;
  MaterialProperty<RealVectorValue> & _stress_normal_vector;
  MaterialProperty<RealVectorValue> & _stress_shear_vector;
};
 

#endif //SOLIDMECHFALCON_H
