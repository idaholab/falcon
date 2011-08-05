#ifndef STRESSSTRAINDAMAGECOMPUTEAUX_H
#define STRESSSTRAINDAMAGECOMPUTEAUX_H

#include "AuxKernel.h"

//libMesh includes
#include "tensor_value.h"
#include "vector_value.h"


//Forward Declarations
class StressStrainDamageComputeAux;

template<>
InputParameters validParams<StressStrainDamageComputeAux>();

/**
 * Coupled auxiliary value
 */
class StressStrainDamageComputeAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all
   * derived classes can be built using the same
   * constructor.
   */
  StressStrainDamageComputeAux(const std::string & name, InputParameters parameters);

  virtual ~StressStrainDamageComputeAux() {}

protected:
  virtual Real computeValue();


  std::string  _quantity_string;

  int _component;
  int _componentb;

  VectorValue<Number> _stress;
  VectorValue<Number> _strain;
  VectorValue<Number> _pstress;
  VectorValue<Number> _pstrain;
  Real                _damage;
  MaterialProperty<Real> & _permeability;
  MaterialProperty<Real> & _damage_coeff;
  MaterialProperty<RealVectorValue> & _stress_normal_vector;
  MaterialProperty<RealVectorValue> & _stress_shear_vector;
  MaterialProperty<RealVectorValue> & _strain_normal_vector;
  MaterialProperty<RealVectorValue> & _strain_shear_vector;
  MaterialProperty<RealVectorValue> & _pstress_normal_vector;
  MaterialProperty<RealVectorValue> & _pstrain_normal_vector;



};

#endif //STRESSSTRAINDAMAGECOMPUTEAUX_H
