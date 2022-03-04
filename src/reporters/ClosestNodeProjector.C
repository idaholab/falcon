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
  InputParameters params = GeneralReporter::validParams();
  params.addClassDescription("Reporter that moves a point and value to the closets node");
  params.addRequiredParam<Point>("point", "Point location to project onto closest node");
  params.addRequiredParam<Real>("value", "Values at point locations to project onto closest node");
  params.addParam<ReporterValueName>(
      "xcoord_name", "x", "Reporter value to create containing x coordinate of point");
  params.addParam<ReporterValueName>(
      "ycoord_name", "y", "Reporter value to create containing y coordinate of point");
  params.addParam<ReporterValueName>(
      "zcoord_name", "z", "Reporter value to create containing z coordinate of point");
  params.addParam<ReporterValueName>(
      "value_name", "value", "Reporter value to create containing value of point");
  params.addParam<Real>("projection_tolerance",
                        libMesh::TOLERANCE,
                        "Search tolerance between point and the closest node.  If a node is not "
                        "found, an error will be produced");
  return params;
}

ClosestNodeProjector::ClosestNodeProjector(const InputParameters & parameters)
  : GeneralReporter(parameters),
    _xcoord(declareValue<Real>("xcoord_name", REPORTER_MODE_REPLICATED)),
    _ycoord(declareValue<Real>("ycoord_name", REPORTER_MODE_REPLICATED)),
    _zcoord(declareValue<Real>("zcoord_name", REPORTER_MODE_REPLICATED)),
    _value(declareValue<Real>("value_name", REPORTER_MODE_REPLICATED, getParam<Real>("value"))),
    _tolerance(getParam<Real>("projection_tolerance"))
{
  Point pt = getParam<Point>("point");
  MooseMesh & mesh = _subproblem.mesh();

  Point pmax(pt(0) + _tolerance, pt(1) + _tolerance, pt(2) + _tolerance);
  Point pmin(pt(0) - _tolerance, pt(1) - _tolerance, pt(2) - _tolerance);
  BoundingBox bbox(pmin, pmax);

  Point closest_node;
  Real nearest_distance = std::numeric_limits<Real>::max();
  for (const auto & node : mesh.getMesh().node_ptr_range())
  {
    if (bbox.contains_point(*node))
    {
      Real distance = (pt - *node).norm();
      if (distance < nearest_distance)
      {
        nearest_distance = distance;
        closest_node = *node;
      }
    }
  }
  if (nearest_distance < _tolerance)
  {
    _xcoord = closest_node(0);
    _ycoord = closest_node(1);
    _zcoord = closest_node(2);
  }
  else
  {
    std::ostringstream errMsg;
    errMsg << "No node located within projection_tolerance= " << _tolerance << " of reporter point "
           << pt;
    mooseError(errMsg.str());
  }
}

void
ClosestNodeProjector::execute()
{
}
