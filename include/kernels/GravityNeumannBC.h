#ifndef GRAVITYNEUMANNBC_H
#define GRAVITYNEUMANNBC_H

#include "BoundaryCondition.h"


//Forward Declarations
class GravityNeumannBC;

template<>
InputParameters valid_params<GravityNeumannBC>();

/**
 * Implements a simple constant GravityNeumann BC where grad(u)=value on the boundary.
 * Uses the term produced from integrating the diffusion operator by parts.
 */
class GravityNeumannBC : public BoundaryCondition
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  GravityNeumannBC(std::string name, InputParameters parameters, std::string var_name, unsigned int boundary_id, std::vector<std::string> coupled_to, std::vector<std::string> coupled_as);
  
virtual ~GravityNeumannBC() {}

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  Real _permeability;
  Real _density;
  Real _mu_w;
  
  RealVectorValue _gravity;
  
private:
};

#endif //GRAVITYNEUMANNBC_H
