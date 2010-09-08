#include "StepDirichletBC.h"
#include "MooseSystem.h"
 
template<>
InputParameters validParams<StepDirichletBC>()
{
  InputParameters params = validParams<BoundaryCondition>();
  params.set<Real>("size")=1.0;
  params.addRequiredParam<std::vector<Real> >("time","The time step ");
  params.addRequiredParam<std::vector<Real> >("value","The values of varable at particular time");

  return params;
}

StepDirichletBC::StepDirichletBC(const std::string & name, MooseSystem & moose_system, InputParameters parameters)
  :BoundaryCondition(name, moose_system, parameters),
   _size(getParam<Real>("size")),
   _time(getParam<std::vector<Real> >("time")),
   _value(getParam<std::vector<Real> >("value"))
{
}

Real
StepDirichletBC::computeQpResidual()
{
  Real u;

   for (unsigned int i=0; i<_size; ++i)
      {
        if(_moose_system._t >= _time[i])
        {
          u = _value[i];
          break;
        }
        
      }
  
  return _u[_qp]- u;
}
