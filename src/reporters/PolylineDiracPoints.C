#include "PolylineDiracPoints.h"
#include "RayTracing.h"
#include "LineSegment.h"
#include "MooseMesh.h"
#include "libmesh/point_locator_base.h"

registerMooseObject("FalconApp", PolylineDiracPoints);

InputParameters
PolylineDiracPoints::validParams()
{
  InputParameters params = GeneralReporter::validParams();

  params.addRequiredParam<std::vector<Point>>(
      "points",
      "Ordered waypoints defining the path, given as a flat list of x y z triplets: '<x0> <y0> "
      "<z0> <x1> <y1> <z1> ...'. Two waypoints define a single straight segment; three or more "
      "define a bent or branching multi-segment polyline. Order matters: list the shallowest "
      "point (eg a well's wellhead) first and the deepest point last, matching the point_file "
      "convention of the PorousFlow line sinks, whose last point is taken to be the well "
      "bottom.");

  params.addRequiredRangeCheckedParam<Real>(
      "weight",
      "weight > 0",
      "The weight reported for every generated point. Its physical meaning is set by the "
      "consuming object; for a PorousFlow line sink or Peaceman borehole it is the borehole "
      "radius (m). A single value applies uniformly along the whole path, matching the "
      "convention of PorousFlowLineGeometry's own 'line_base' input mode.");

  params.addRangeCheckedParam<Real>(
      "coincident_point_tolerance",
      1e-6,
      "coincident_point_tolerance >= 0",
      "Absolute length (in mesh length units) below which a path sub-segment inside a single "
      "element, or the gap between two consecutively generated points, is treated as zero and "
      "the point is dropped. This suppresses the zero-length trailing sub-segment that "
      "Moose::elementsIntersectedByLine emits whenever a segment ends exactly on an element "
      "face, and guards against a near-tangential path producing coincident points, which the "
      "consuming line sink would reject as a zero-length segment.");

  params.addClassDescription(
      "Reports the coordinates and a uniform weight of one point per mesh element crossed by a "
      "polyline path defined by an ordered list of waypoints, for consumption via the "
      "x_coord_reporter/y_coord_reporter/z_coord_reporter/weight_reporter parameters of a "
      "PorousFlow line sink. Generalizes PorousFlowLineGeometry's single-straight-segment "
      "'line_base' input mode to an arbitrary bent or branching multi-segment path specified by "
      "literal endpoint coordinates, on 2D or 3D meshes alike.");

  return params;
}

PolylineDiracPoints::PolylineDiracPoints(const InputParameters & parameters)
  : GeneralReporter(parameters),
    _points(getParam<std::vector<Point>>("points")),
    _weight_value(getParam<Real>("weight")),
    _tolerance(getParam<Real>("coincident_point_tolerance")),
    _x(declareValueByName<std::vector<Real>>("x", REPORTER_MODE_REPLICATED)),
    _y(declareValueByName<std::vector<Real>>("y", REPORTER_MODE_REPLICATED)),
    _z(declareValueByName<std::vector<Real>>("z", REPORTER_MODE_REPLICATED)),
    _w(declareValueByName<std::vector<Real>>("weight", REPORTER_MODE_REPLICATED))
{
  if (_points.size() < 2)
    paramError(
        "points", "At least two waypoints are needed to define a path; got ", _points.size(), ".");

  for (std::size_t j = 0; j + 1 < _points.size(); ++j)
    if ((_points[j + 1] - _points[j]).norm() <= _tolerance)
      paramError("points",
                 "Waypoints ",
                 j,
                 " and ",
                 j + 1,
                 " (",
                 _points[j],
                 " and ",
                 _points[j + 1],
                 ") are coincident, giving a zero-length path segment. Remove the duplicate.");
}

void
PolylineDiracPoints::initialSetup()
{
  generatePoints();
}

void
PolylineDiracPoints::meshChanged()
{
  generatePoints();
}

void
PolylineDiracPoints::generatePoints()
{
  MooseMesh & moose_mesh = _subproblem.mesh();

  // Moose::elementsIntersectedByLine walks element-to-element through shared faces, which cannot
  // cross a partition boundary: on a distributed mesh each processor's walk would stop at the
  // edge of its own (plus one ghost layer) portion and report a different, truncated point set,
  // violating this Reporter's REPORTER_MODE_REPLICATED contract - and a consuming DiracKernel
  // would then evaluate System::point_value() at different coordinates on different ranks, which
  // is a collective call that requires every rank to agree on the point. This is the same
  // constraint PorousFlowLineGeometry's own "line_base" mode has; made explicit here rather than
  // left to fail confusingly.
  if (moose_mesh.isDistributedMesh())
    mooseError("PolylineDiracPoints requires a replicated mesh (Mesh/parallel_type=replicated, "
               "the default). The element-by-element walk used to find the crossed elements "
               "cannot traverse a distributed mesh.");

  auto point_locator = moose_mesh.getPointLocator();
  point_locator->enable_out_of_mesh_mode();

  _x.clear();
  _y.clear();
  _z.clear();
  _w.clear();

  for (std::size_t j = 0; j + 1 < _points.size(); ++j)
  {
    const Point & a = _points[j];
    const Point & b = _points[j + 1];

    std::vector<Elem *> elems;
    std::vector<LineSegment> segs;
    Moose::elementsIntersectedByLine(a, b, moose_mesh, *point_locator, elems, segs);

    // One candidate point per crossed element: the midpoint of the path inside that element.
    std::vector<Point> seg_points;
    for (const auto & seg : segs)
    {
      // elementsIntersectedByLine emits a zero-length trailing sub-segment whenever the segment
      // ends exactly on an element face (it steps through that face before discovering the line
      // has run out). Filtered by length, not by an exact Point ==, because the mesh node's
      // coordinate need not be bitwise equal to the waypoint the user typed.
      if ((seg.end() - seg.start()).norm() <= _tolerance)
        continue;
      seg_points.push_back((seg.start() + seg.end()) * 0.5);
    }

    if (seg_points.empty())
      paramError("points",
                 "No mesh element was found along the path segment from ",
                 a,
                 " to ",
                 b,
                 ". Both waypoints must lie inside the mesh.");

    // Pin the segment to its own two waypoints, exactly as
    // PorousFlowLineGeometry::regenPoints() does for its single line_base segment, so the
    // generated polyline reproduces the requested path length exactly and, for a well, puts a
    // point precisely at each end of this segment.
    seg_points.front() = a;
    if (seg_points.size() > 1)
      seg_points.back() = b;
    else
      // A segment crossing a single element would otherwise collapse to just its start point.
      seg_points.push_back(b);

    // Interior-waypoint (junction) de-duplication: segment j-1 was pinned to END at exactly
    // _points[j], and segment j is pinned to START at exactly _points[j], so for j > 0 the first
    // entry here is always a bitwise duplicate of the last point already emitted and is skipped.
    // This is a structural rule, not a geometric one - both copies are the same entry of the
    // 'points' parameter, never the result of a floating-point computation - so no tolerance
    // comparison is needed for it.
    const std::size_t first = (j == 0) ? 0 : 1;

    for (std::size_t k = first; k < seg_points.size(); ++k)
    {
      // Defensive net for the one case the structural rule above cannot cover: a near-tangential
      // path whose sub-segment inside an element is itself vanishingly short, leaving an interior
      // midpoint arbitrarily close to a pinned waypoint. The consuming
      // PorousFlowLineGeometry::calcLineLengths() mooseErrors on a zero-length consecutive-point
      // gap, so dropping the redundant point here is strictly better than failing there.
      if (!_x.empty() &&
          (Point(_x.back(), _y.back(), _z.back()) - seg_points[k]).norm() <= _tolerance)
        continue;

      _x.push_back(seg_points[k](0));
      _y.push_back(seg_points[k](1));
      _z.push_back(seg_points[k](2));
      _w.push_back(_weight_value);
    }
  }

  if (_x.size() < 2)
    paramError("points",
               "Only ",
               _x.size(),
               " point(s) were generated along the requested path. A line sink needs at least "
               "two.");
}
