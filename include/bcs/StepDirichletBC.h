#ifndef STEPDIRICHLETBC_H
#define STEPDIRICHLETBC_H

#include "libmesh_common.h"
#include "BoundaryCondition.h"


//Forward Declarations
class StepDirichletBC;

template<>
InputParameters validParams<StepDirichletBC>();

class StepDirichletBC : public BoundaryCondition
{
public:

  StepDirichletBC(std::string name, MooseSystem & moose_system, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();

private:
  std::vector<Real> _time;
  std::vector<Real> _value;
  Real _size;
  
};
 
#endif
