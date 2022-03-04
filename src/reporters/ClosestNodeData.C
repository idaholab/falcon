//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "ClosestNodeData.h"
#include "MooseUtils.h"

registerMooseObject("FalconApp", ClosestNodeData);

InputParameters
ClosestNodeData::validParams()
{
  InputParameters params = ClosestNodeBase::validParams();
  params.addClassDescription("Reports variable data from the closest node to some point");
  params.addRequiredParam<VariableName>("variable", "The variable to be monitored at the node");

  return params;
}

ClosestNodeData::ClosestNodeData(const InputParameters & parameters)
  : ClosestNodeBase(parameters),
    _var_name(parameters.get<VariableName>("variable")),
    _var(declareValueByName<Real>(_var_name, REPORTER_MODE_REPLICATED))
{
}

void
ClosestNodeData::execute()
{
  Real value = 0;
  if (_node_ptr && _node_ptr->processor_id() == processor_id())
    value = _subproblem.getStandardVariable(_tid, _var_name).getNodalValue(*_node_ptr);

  gatherSum(value);
  _var = value;
}
