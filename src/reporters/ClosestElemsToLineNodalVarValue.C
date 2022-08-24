//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ClosestElemsToLineNodalVarValue.h"
#include "MooseMesh.h"
#include "MooseVariable.h"

registerMooseObject("FalconApp", ClosestElemsToLineNodalVarValue);

InputParameters
ClosestElemsToLineNodalVarValue::validParams()
{
  InputParameters params = ClosestElemsToLine::validParams();
  params.addClassDescription("Creates a real_vector_names reporter that has each "
                             "elements average nodal variable value.");
  params.addRequiredParam<VariableName>("variable_to_sample", "The nodal variable to monitor.");
  return params;
}

ClosestElemsToLineNodalVarValue::ClosestElemsToLineNodalVarValue(const InputParameters & parameters)
  : ClosestElemsToLine(parameters),
    MooseVariableInterface<Real>(this,
                                 false,
                                 "variable_to_sample",
                                 Moose::VarKindType::VAR_ANY,
                                 Moose::VarFieldType::VAR_FIELD_STANDARD),
    _var_name(parameters.get<VariableName>("variable_to_sample")),
    _var(_subproblem.getStandardVariable(_tid, _var_name)),
    _value(declareValueByName<std::vector<Real>>(_var_name, REPORTER_MODE_REPLICATED))
{
  if (!_var.isNodal())
    mooseError("variable_to_sample must be a nodal variable.");
}

void
ClosestElemsToLineNodalVarValue::initialSetup()
{
  ClosestElemsToLine::initialSetup();
  _value.resize(_eid.size(), std::numeric_limits<Real>::max());
}

void
ClosestElemsToLineNodalVarValue::execute()
{
  const MooseMesh & mesh = _subproblem.mesh();
  for (size_t i = 0; i < _eid.size(); ++i)
  {
    Real value = 0.0;
    const Elem * elem = mesh.getMesh().query_elem_ptr(_eid[i]);
    if (elem->processor_id() == processor_id())
    {
      size_t n = 0;
      for (auto & node : elem->node_ref_range())
      {
        value += _var.getNodalValue(node);
        n++;
      }
      if (n > 0)
        value /= (Real)n;
    }
    _value[i] = value;
  }
}

void
ClosestElemsToLineNodalVarValue::finalize()
{
    //_value should be zero on every proc so this will just communicate it.
    for (size_t i = 0; i < _eid.size(); ++i)
      gatherSum(_value[i]);
}
