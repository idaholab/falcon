# PolylineDiracPoints

!syntax description /Reporters/PolylineDiracPoints

## Overview

`PorousFlowLineGeometry` (the base class shared by `PorousFlowPeacemanBorehole`,
`PorousFlowPolyLineSink`, [PorousFlowCasedBoreholeHeatExchange.md], and other PorousFlow line
sinks) supports three mutually-exclusive ways to place its Dirac points: a hand-written
`point_file`, whose spacing the user must keep in step with the mesh by hand; `line_base`/
`line_direction`/`line_length`, which auto-places one point per mesh element crossed but only
along a *single* straight segment, expressed as a base point plus a direction vector and a
length; and reporter-based input (`x_coord_reporter`/`y_coord_reporter`/`z_coord_reporter`/
`weight_reporter`).

`PolylineDiracPoints` fills that reporter-based slot with the auto-placement behavior of
`line_base`, generalized to an **arbitrary path given as literal waypoint coordinates**: two
waypoints define one straight segment (the `line_base` case, expressed by both endpoints
instead of a base+direction+length decomposition); three or more define a path with a bend at
each interior waypoint, useful for a deviated or multi-lateral well. Nothing in the placement
algorithm inspects the mesh's topological dimension - it operates on raw 3D point geometry via
`Moose::elementsIntersectedByLine` - so a 2D or 3D mesh is handled identically.

### Point placement

For each consecutive pair of waypoints, one point is placed at the midpoint of the path inside
every mesh element that segment crosses, except that the very first and very last point of each
segment are pinned to that segment's own two waypoints exactly (matching
`PorousFlowLineGeometry::regenPoints()`'s own convention for its single `line_base` segment). A
two-waypoint path crossing $N$ elements therefore produces $N$ points.

At an interior waypoint (a bend), the segment ending there and the segment starting there are
each pinned to that same waypoint, which would otherwise duplicate it. `PolylineDiracPoints`
detects this structurally (both pinned values come from the same entry of the `points`
parameter, not from a floating-point computation) and emits the junction point exactly once. A
small tolerance-based check (`coincident_point_tolerance`) additionally guards a near-tangential
path whose interior midpoint could otherwise land arbitrarily close to a pinned waypoint - a
case the structural rule cannot see, and which the consuming `PorousFlowLineGeometry`'s
`calcLineLengths()` would `mooseError` on as a zero-length segment.

The same tolerance also filters the zero-length trailing sub-segment that
`Moose::elementsIntersectedByLine` reliably emits whenever a segment ends exactly on an element
face (the ray-tracing walk steps through that face before discovering the line has run out).

### Ordering convention

List waypoints shallowest/first end first, deepest/last end last. This matters because a
consuming `PorousFlowLineGeometry` treats its very *last* point as the well bottom
(`_bottom_point`, used by e.g. `PorousFlowPeacemanBorehole`'s `bottom_p_or_t`), the same
convention as a hand-written `point_file`.

## Limitations

- Requires a replicated mesh (the default); the element-by-element walk cannot traverse a
  distributed mesh, the same restriction `line_base` mode already has.
- A single scalar `weight` applies uniformly along the whole path - there is no per-point radius
  in this input mode.
- The point set is regenerated on `meshChanged()`, but a consuming `PorousFlowLineGeometry` will
  not re-read it (its own `meshChanged()`/`regenPoints()` is a no-op in reporter mode), so mesh
  adaptivity is not supported end-to-end through this path.

## Example Input File Syntax

!listing examples/geothermal_wellbore/wells_variable_unit_weight.i block=Reporters/cased_path

!syntax parameters /Reporters/PolylineDiracPoints

!syntax inputs /Reporters/PolylineDiracPoints

!syntax children /Reporters/PolylineDiracPoints
