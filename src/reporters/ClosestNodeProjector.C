//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "ClosestNodeProjector.h"
#include "MooseUtils.h"

registerMooseObject("FalconApp", ClosestNodeProjector);

InputParameters
ClosestNodeProjector::validParams()
{
  InputParameters params = ClosestNode::validParams();
  params.addClassDescription("Reporter that moves a point and value to the closets node");
  params.addRequiredParam<ReporterName>(
      "point_value",
      "input reporter of value to project onto closest node.  This uses "
      "the reporter syntax <reporter>/<name>.");
  return params;
}

ClosestNodeProjector::ClosestNodeProjector(const InputParameters & parameters)
  : ClosestNode(parameters),
    _point_value(getReporterValue<std::vector<Real>>("point_value", REPORTER_MODE_REPLICATED)),
    _node_value(declareValueByName<std::vector<Real>>("node_value", REPORTER_MODE_REPLICATED))
{
  std::copy(_point_value.begin(), _point_value.end(), back_inserter(_node_value));
}
