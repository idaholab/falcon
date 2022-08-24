//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ClosestElemsToLineVarValue.h"

// MOOSE includes
#include "MooseMesh.h"
#include "MooseVariable.h"
#include "SubProblem.h"

registerMooseObject("FalconApp", ClosestElemsToLineVarValue);

InputParameters
ClosestElemsToLineVarValue::validParams()
{
  InputParameters params = ClosestElemsToLine::validParams();
  params.addClassDescription("Creates a real_vector_names reporter that has each "
                             "elements average nodal variable value.");
  params.addRequiredParam<VariableName>("variable_to_sample", "The nodal variable to monitor.");
  return params;
}

ClosestElemsToLineVarValue::ClosestElemsToLineVarValue(const InputParameters & parameters)
  : ClosestElemsToLine(parameters),
    _var_name(parameters.get<VariableName>("variable_to_sample")),
    _value(declareValueByName<std::vector<Real>>(_var_name, REPORTER_MODE_REPLICATED))
{
}

void
ClosestElemsToLineVarValue::initialSetup()
{
  ClosestElemsToLine::initialSetup();
  _value.resize(_eid.size(), std::numeric_limits<Real>::max());
}

void
ClosestElemsToLineVarValue::finalize()
{
  const MooseMesh & mesh = _subproblem.mesh();
  for (size_t i = 0; i < _eid.size(); ++i)
  {
    Real value = 0.0;
    const Elem * elem = mesh.getMesh().query_elem_ptr(_eid[i]);
    if (elem->processor_id() == processor_id())
    {
      _fe_problem.setCurrentSubdomainID(elem, 0);
      _subproblem.prepare(elem, _tid);
      _subproblem.reinitElem(elem, _tid);

      MooseVariableField<Real> & var = static_cast<MooseVariableField<Real> &>(
          _subproblem.getActualFieldVariable(_tid, _var_name));
      const VariableValue & u = var.sln();
      unsigned int n = u.size();
      for (unsigned int i = 0; i < n; i++)
        value += u[i];
      value /= n;
    }
    _value[i] = value;
  }
  //_value should be zero on every proc so this will just communicate it.
  for (size_t i = 0; i < _eid.size(); ++i)
    gatherSum(_value[i]);
}
