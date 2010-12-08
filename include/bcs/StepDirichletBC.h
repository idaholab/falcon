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

  StepDirichletBC(const std::string & name, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();

private:
  Real _size;
  std::vector<Real> _time;
  std::vector<Real> _value;  
};
 
#endif
