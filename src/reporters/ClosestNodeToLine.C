//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ClosestNodeToLine.h"

registerMooseObject("FalconApp", ClosestNodeToLine);

InputParameters
ClosestNodeToLine::validParams()
{
  InputParameters params = GeneralReporter::validParams();
  params.addClassDescription("Reports coordinates and element ids of the closest nodes to a line "
                             "with unique domain_name values. ");
  params.addRequiredParam<ReporterName>("point_x1",
                                        "input reporter of x-coordinate name for point.  This uses "
                                        "the reporter syntax <reporter>/<name>.");
  params.addRequiredParam<ReporterName>("point_y1",
                                        "input reporter of y-coordinate name for point.  This uses "
                                        "the reporter syntax <reporter>/<name>.");
  params.addRequiredParam<ReporterName>("point_z1",
                                        "input reporter of z-coordinate name for point.  This uses "
                                        "the reporter syntax <reporter>/<name>.");
  params.addRequiredParam<ReporterName>("point_x2",
                                        "input reporter of x-coordinate name for point.  This uses "
                                        "the reporter syntax <reporter>/<name>.");
  params.addRequiredParam<ReporterName>("point_y2",
                                        "input reporter of y-coordinate name for point.  This uses "
                                        "the reporter syntax <reporter>/<name>.");
  params.addRequiredParam<ReporterName>("point_z2",
                                        "input reporter of z-coordinate name for point.  This uses "
                                        "the reporter syntax <reporter>/<name>.");
  params.addRequiredParam<VariableName>("domain_name",
                                        "Name for the variable in domain to determine uniqueness");
  params.addParam<Real>("projection_tolerance",
                        libMesh::TOLERANCE,
                        "Search tolerance between line and the closest node.  If a node is not "
                        "found, an error will be produced");
  return params;
}

ClosestNodeToLine::ClosestNodeToLine(const InputParameters & parameters)
  : GeneralReporter(parameters),
    _point_x1(getReporterValue<std::vector<Real>>("point_x1", REPORTER_MODE_REPLICATED)),
    _point_y1(getReporterValue<std::vector<Real>>("point_y1", REPORTER_MODE_REPLICATED)),
    _point_z1(getReporterValue<std::vector<Real>>("point_z1", REPORTER_MODE_REPLICATED)),
    _point_x2(getReporterValue<std::vector<Real>>("point_x2", REPORTER_MODE_REPLICATED)),
    _point_y2(getReporterValue<std::vector<Real>>("point_y2", REPORTER_MODE_REPLICATED)),
    _point_z2(getReporterValue<std::vector<Real>>("point_z2", REPORTER_MODE_REPLICATED)),
    _tolerance(getParam<Real>("projection_tolerance")),
    _eid(declareValueByName<std::vector<dof_id_type>>("elem_id", REPORTER_MODE_REPLICATED)),
    _pt_x(declareValueByName<std::vector<Real>>("point_x", REPORTER_MODE_REPLICATED)),
    _pt_y(declareValueByName<std::vector<Real>>("point_y", REPORTER_MODE_REPLICATED)),
    _pt_z(declareValueByName<std::vector<Real>>("point_z", REPORTER_MODE_REPLICATED)),
    _p1(Point(_point_x1[0], _point_y1[0], _point_z1[0])),
    _p2(Point(_point_x2[0], _point_y2[0], _point_z2[0])),
    _line(_p2 - _p1),
    _dom_name(parameters.get<VariableName>("domain_name"))
{
}

void
ClosestNodeToLine::execute()
{
  const MooseMesh & mesh = _subproblem.mesh();
  for (const auto & elem : as_range(mesh.getMesh().active_local_elements_begin(),
                                    mesh.getMesh().active_local_elements_end()))
  {
    Real curr_dist = getDistance(*elem);
    if (curr_dist < _tolerance)
    {
      addPoint(*elem, curr_dist);
    }
  }
}

void
ClosestNodeToLine::finalize()
{
  std::vector<LocData> loc_datas;

  for (const auto & l_d : _dom_map)
    loc_datas.push_back(l_d.second);
  if (_app.n_processors() > 1)
  {
    _communicator.allgather(loc_datas);
  }

  convertToMap(loc_datas);
  convertVecs();
}

Real
ClosestNodeToLine::getDistance(const Elem & elem)
{
  // distance between node and line. This assumes the line goes on forever.
  Point term1 = elem.vertex_average() - _p1;
  Point term2 = elem.vertex_average() - _p2;
  Point cross = term1.cross(term2);
  return cross.norm() / _line.norm();
}

void
ClosestNodeToLine::addPoint(const Elem & elem, Real dist)
{
  auto value = _subproblem.getStandardVariable(_tid, _dom_name).getElementalValue(&elem);

  int value_int = (int)std::round(value);
  if (!_dom_map.count(value_int))
    // if this subdomain has not been added yet
    _dom_map[value_int] = LocData(value_int, elem.id(), dist, elem.vertex_average());
  else if (dist < _dom_map[value_int].distance)
    // if this subdomain has been added, but the new distance is closer
    _dom_map[value_int] = LocData(value_int, elem.id(), dist, elem.vertex_average());
}
void
ClosestNodeToLine::convertVecs()
{
  for (auto & [frac_num, loc_data] : _dom_map)
  {
    _eid.push_back(loc_data.elem_id);
    _pt_x.push_back(loc_data.loc(0));
    _pt_y.push_back(loc_data.loc(1));
    _pt_z.push_back(loc_data.loc(2));
  }
  if (_eid.size() == 0)
  {
    std::ostringstream errMsg;
    errMsg << "No node located within projection_tolerance= " << _tolerance
           << " of reporter line from point 1: " << _p1 << " to point 2:" << _p2;
    mooseError(errMsg.str());
  }
  _dom_map.clear();
}

void
ClosestNodeToLine::convertToMap(std::vector<LocData> & data)
{
  _dom_map.clear();

  for (auto & loc_data : data)
  {
    if (!_dom_map.count(loc_data.frac_num))
      _dom_map[loc_data.frac_num] = loc_data;
    else if (loc_data.distance < _dom_map[loc_data.frac_num].distance)
      _dom_map[loc_data.frac_num] = loc_data;
  }
  data.clear();
}

namespace TIMPI
{
StandardType<ClosestNodeToLine::LocData>::StandardType(const ClosestNodeToLine::LocData * example)
{
  static const ClosestNodeToLine::LocData p;
  if (!example)
    example = &p;

#ifdef LIBMESH_HAVE_MPI
  // Get the sub-data-types, and make sure they live long enough
  // to construct the derived type
  StandardType<int> d1(&example->frac_num);
  StandardType<dof_id_type> d2(&example->elem_id);
  StandardType<Real> d3(&example->distance);
  StandardType<Point> d4(&example->loc);

  MPI_Datatype types[] = {(data_type)d1, (data_type)d2, (data_type)d3, (data_type)d4};
  int blocklengths[] = {1, 1, 1, 1};
  MPI_Aint displs[4], start;
  libmesh_call_mpi(MPI_Get_address(const_cast<ClosestNodeToLine::LocData *>(example), &start));
  libmesh_call_mpi(MPI_Get_address(const_cast<int *>(&example->frac_num), &displs[0]));
  libmesh_call_mpi(MPI_Get_address(const_cast<dof_id_type *>(&example->elem_id), &displs[1]));
  libmesh_call_mpi(MPI_Get_address(const_cast<Real *>(&example->distance), &displs[2]));
  libmesh_call_mpi(MPI_Get_address(const_cast<Point *>(&example->loc), &displs[3]));

  for (std::size_t i = 0; i < 4; ++i)
    displs[i] -= start;

  MPI_Datatype tmptype;
  libmesh_call_mpi(MPI_Type_create_struct(4, blocklengths, displs, types, &tmptype));
  libmesh_call_mpi(MPI_Type_commit(&tmptype));

  // resize the structure type to account for padding, if any
  libmesh_call_mpi(
      MPI_Type_create_resized(tmptype, 0, sizeof(ClosestNodeToLine::LocData), &_datatype));
  libmesh_call_mpi(MPI_Type_free(&tmptype));

  this->commit();
#endif // LIBMESH_HAVE_MPI
}
} // namespace TIMPI

StandardType<ClosestNodeToLine::LocData>::StandardType(
    const StandardType<ClosestNodeToLine::LocData> & t)
{
#ifdef LIBMESH_HAVE_MPI
  libmesh_call_mpi(MPI_Type_dup(t._datatype, &_datatype));
#endif
}