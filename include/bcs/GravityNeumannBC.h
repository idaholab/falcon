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

#ifndef GRAVITYNEUMANNBC_H
#define GRAVITYNEUMANNBC_H

#include "IntegratedBC.h"


//Forward Declarations
class GravityNeumannBC;

template<>
InputParameters validParams<GravityNeumannBC>();

/**
 * Implements a simple constant GravityNeumann BC where grad(u)=value on the boundary.
 * Uses the term produced from integrating the diffusion operator by parts.
 */
class GravityNeumannBC : public IntegratedBC
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  GravityNeumannBC(const std::string & name, InputParameters parameters);

virtual ~GravityNeumannBC() {}

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  const MaterialProperty<Real> & _permeability;
  const MaterialProperty<Real> & _density_water;
  const MaterialProperty<Real> & _viscosity_water;
  const MaterialProperty<Real> & _gravity;
  const MaterialProperty<RealVectorValue> & _gravity_vector;

private:
};

#endif //GRAVITYNEUMANNBC_H
