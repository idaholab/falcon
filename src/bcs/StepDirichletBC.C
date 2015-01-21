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

#include "StepDirichletBC.h"

template<>
InputParameters validParams<StepDirichletBC>()
{
  InputParameters params = validParams<NodalBC>();
  params.set<Real>("size")=1.0;
  params.addRequiredParam<std::vector<Real> >("time","The time step ");
  params.addRequiredParam<std::vector<Real> >("value","The values of variable at particular time");

  return params;
}

StepDirichletBC::StepDirichletBC(const std::string & name, InputParameters parameters)
  :NodalBC(name, parameters),
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
        if(_t >= _time[i])
        {
          u = _value[i];
          break;
        }

      }

  return _u[_qp]- u;
}
