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

  Real _permeability;
  Real _density;
  Real _viscosity_water;
  
  RealVectorValue _gravity;
  
private:
};

#endif //GRAVITYNEUMANNBC_H
