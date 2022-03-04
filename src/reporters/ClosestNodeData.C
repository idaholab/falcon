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
  InputParameters params = GeneralReporter::validParams();
  params.addClassDescription("Reports variable data from the closest node to some point");
  params.addRequiredParam<Point>("point", "Location of point to project onto closest node");
  params.addRequiredParam<VariableName>("variable", "The variable to be monitored at the node");
  params.addParam<Real>("projection_tolerance",
                        libMesh::TOLERANCE,
                        "Search tolerance between point and the closest node.  If a node is not "
                        "found, an error will be produced");
  return params;
}

ClosestNodeData::ClosestNodeData(const InputParameters & parameters)
  : GeneralReporter(parameters),
    _var_name(parameters.get<VariableName>("variable")),
    _xcoord(declareValueByName<Real>("xcoord", REPORTER_MODE_REPLICATED)),
    _ycoord(declareValueByName<Real>("ycoord", REPORTER_MODE_REPLICATED)),
    _zcoord(declareValueByName<Real>("zcoord", REPORTER_MODE_REPLICATED)),
    _tolerance(getParam<Real>("projection_tolerance")),
    _var(declareValueByName<Real>(_var_name, REPORTER_MODE_REPLICATED)),
    _nid(declareValueByName<dof_id_type>("node_id", REPORTER_MODE_REPLICATED))
{
  Point pt = getParam<Point>("point");
  MooseMesh & mesh = _subproblem.mesh();

  Point pmax(pt(0) + _tolerance, pt(1) + _tolerance, pt(2) + _tolerance);
  Point pmin(pt(0) - _tolerance, pt(1) - _tolerance, pt(2) - _tolerance);
  BoundingBox bbox(pmin, pmax);

  const Node * closest_node;
  Real nearest_distance = std::numeric_limits<Real>::max();
  for (const auto & node : mesh.getMesh().node_ptr_range())
  {
    if (bbox.contains_point(*node))
    {
      Real distance = (pt - *node).norm();
      if (distance < nearest_distance)
      {
        closest_node = node;
        nearest_distance = distance;
      }
    }
  }

  if (nearest_distance <= _tolerance)
  {
    _node_ptr = closest_node;
    _xcoord = (*closest_node)(0);
    _ycoord = (*closest_node)(1);
    _zcoord = (*closest_node)(2);
    _nid = closest_node->id();
  }
  else
  {
    std::ostringstream errMsg;
    errMsg << "No nodes located within projection_tolerance= " << _tolerance
           << " of reporter point " << pt;
    mooseError(errMsg.str());
  }
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
