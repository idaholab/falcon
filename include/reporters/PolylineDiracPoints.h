#pragma once

#include "GeneralReporter.h"

/**
 * Reports the coordinates and a uniform weight of one point per mesh element crossed by a
 * polyline path defined by an ordered list of waypoints, for consumption via the
 * x_coord_reporter/y_coord_reporter/z_coord_reporter/weight_reporter parameters of
 * PorousFlowLineGeometry (and its subclasses: PorousFlowPeacemanBorehole,
 * PorousFlowCasedBoreholeHeatExchange, etc.).
 *
 * Generalizes PorousFlowLineGeometry's own "line_base"/"line_direction"/"line_length" input mode
 * (which auto-places one point per element crossed, but only along a single straight segment
 * given as a base point plus a direction vector and length) to an arbitrary bent or branching
 * multi-segment path given as literal waypoint coordinates: two waypoints define one straight
 * segment (equivalent to "line_base", but expressed by both endpoints rather than a
 * base+direction+length decomposition); three or more define a path with one bend per interior
 * waypoint. Nothing here depends on the mesh's topological dimension - Moose::elementsIntersectedByLine
 * operates on raw 3D Point geometry - so a 2D or 3D mesh is handled identically.
 *
 * The work is done in initialSetup() (and again in meshChanged()), not execute(): a consuming
 * PorousFlowLineGeometry reads this Reporter's declared vectors from its own initialSetup(), to
 * size its own Dirac point list and compute its own segment-length weights, and MOOSE registers
 * no execute()-time dependency between a DiracKernel and a Reporter it reads from. Filling the
 * vectors in initialSetup() relies instead on FEProblemBase::initialSetup()'s fixed ordering,
 * which runs every UserObject's (hence every Reporter's) initialSetup() before any solver
 * system's initialSetup() - which is what calls DiracKernel::initialSetup().
 */
class PolylineDiracPoints : public GeneralReporter
{
public:
  static InputParameters validParams();

  PolylineDiracPoints(const InputParameters & parameters);

  virtual void initialSetup() override;
  virtual void meshChanged() override;

  virtual void initialize() override {}
  virtual void execute() override {}
  virtual void finalize() override {}

protected:
  /// (Re)computes _x/_y/_z/_w from _points and the current mesh
  void generatePoints();

  /// Ordered waypoints defining the path: shallowest/first end first, deepest/last end last
  const std::vector<Point> & _points;

  /// Weight reported at every generated point (eg a borehole radius, for a line-sink consumer)
  const Real _weight_value;

  /// Length below which a path sub-segment inside one element, or the gap between two
  /// consecutively generated points, is treated as zero and dropped
  const Real _tolerance;

  ///@{
  /// Generated point coordinates and weights, one entry per mesh element crossed
  std::vector<Real> & _x;
  std::vector<Real> & _y;
  std::vector<Real> & _z;
  std::vector<Real> & _w;
  ///@}
};
