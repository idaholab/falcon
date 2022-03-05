//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "ClosestNode.h"
#include "MooseUtils.h"

registerMooseObject("FalconApp", ClosestNode);

InputParameters
ClosestNode::validParams()
{
  InputParameters params = GeneralReporter::validParams();
  params.addClassDescription(
      "Reports coordinates and node id of the node closest to the input point");
  params.addRequiredParam<ReporterName>("point_x",
                                        "input reporter of x-coordinate name for point.  This uses "
                                        "the reporter syntax <reporter>/<name>.");
  params.addRequiredParam<ReporterName>("point_y",
                                        "input reporter of y-coordinate name for point.  This uses "
                                        "the reporter syntax <reporter>/<name>.");
  params.addRequiredParam<ReporterName>("point_z",
                                        "input reporter of z-coordinate name for point.  This uses "
                                        "the reporter syntax <reporter>/<name>.");
  params.addParam<Real>("projection_tolerance",
                        libMesh::TOLERANCE,
                        "Search tolerance between point and the closest node.  If a node is not "
                        "found, an error will be produced");
  return params;
}

ClosestNode::ClosestNode(const InputParameters & parameters)
  : GeneralReporter(parameters),
    _point_x(getReporterValue<std::vector<Real>>("point_x", REPORTER_MODE_REPLICATED)),
    _point_y(getReporterValue<std::vector<Real>>("point_y", REPORTER_MODE_REPLICATED)),
    _point_z(getReporterValue<std::vector<Real>>("point_z", REPORTER_MODE_REPLICATED)),
    _tolerance(getParam<Real>("projection_tolerance")),
    _nid(declareValueByName<std::vector<dof_id_type>>("node_id", REPORTER_MODE_REPLICATED)),
    _node_x(declareValueByName<std::vector<Real>>("node_x", REPORTER_MODE_REPLICATED)),
    _node_y(declareValueByName<std::vector<Real>>("node_y", REPORTER_MODE_REPLICATED)),
    _node_z(declareValueByName<std::vector<Real>>("node_z", REPORTER_MODE_REPLICATED))
{
  initialize();
}

void
ClosestNode::initialize()
{
  _node_ptrs.clear();
  _node_x.clear();
  _node_y.clear();
  _node_z.clear();
  _nid.clear();

  for (size_t i = 0; i < _point_x.size(); ++i)
  {
    Point pt(_point_x[i], _point_y[i], _point_z[i]);
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
          nearest_distance = distance;
          closest_node = node;
        }
      }
    }
    if (nearest_distance < _tolerance)
    {
      _node_ptrs.push_back(closest_node);
      _node_x.push_back((*closest_node)(0));
      _node_y.push_back((*closest_node)(1));
      _node_z.push_back((*closest_node)(2));
      _nid.push_back(closest_node->id());
    }
    else
    {
      std::ostringstream errMsg;
      errMsg << "No node located within projection_tolerance= " << _tolerance
             << " of reporter point " << pt;
      mooseError(errMsg.str());
    }
  }
}
