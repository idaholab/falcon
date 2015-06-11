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

#ifndef GRAVITY_H
#define GRAVITY_H

#include "Kernel.h"
#include "Material.h"

//libMesh includes
#include "libmesh/tensor_value.h"
#include "libmesh/vector_value.h"


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
  const MaterialProperty<Real> & _density;
  const MaterialProperty<Real> & _gravity;
  const MaterialProperty<RealVectorValue> & _gravity_vector;

};


#endif //GRAVITY_H
