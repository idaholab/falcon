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

#ifndef STRESSSTRAINDAMAGECOMPUTEAUX_H
#define STRESSSTRAINDAMAGECOMPUTEAUX_H

#include "AuxKernel.h"
#include "MooseEnum.h"

//libMesh includes
#include "libmesh/tensor_value.h"
#include "libmesh/vector_value.h"


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

  const unsigned int _mesh_dimension;

  std::string  _quantity_string;

  int _component;

  VectorValue<Number> _stress;
  VectorValue<Number> _strain;

  MaterialProperty<RealVectorValue> & _stress_normal_vector;
  MaterialProperty<RealVectorValue> & _stress_shear_vector;
  MaterialProperty<RealVectorValue> & _strain_normal_vector;
  MaterialProperty<RealVectorValue> & _strain_shear_vector;

};

#endif //STRESSSTRAINDAMAGECOMPUTEAUX_H
