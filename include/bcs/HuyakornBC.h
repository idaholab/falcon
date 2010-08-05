#ifndef HUYAKORNBC_H
#define HUYAKORNBC_H

#include "BoundaryCondition.h"


//Forward Declarations
class HuyakornBC;

template<>
InputParameters validParams<HuyakornBC>();

/**
 * Implements a simple constant Huyakorn BC where grad(u)=value on the boundary.
 * Uses the term produced from integrating the diffusion operator by parts.
 */
class HuyakornBC : public BoundaryCondition
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  HuyakornBC(std::string name, MooseSystem & moose_system, InputParameters parameters);
  
virtual ~HuyakornBC() {}

protected:
  virtual Real computeQpResidual();
  
  MaterialProperty<Real> &_lamda;
  MaterialProperty<Real> &_km;
  MaterialProperty<Real> &_dTbydP_H;
  VariableGradient & _grad_p;  
private:
  Real _value;
};

#endif //HUYAKORNBC_H
