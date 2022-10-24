/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c)     2012 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "MarkCutElems.h"
#include "TraceRayTools.h"
#include "libmesh/mesh_tools.h"

registerMooseObject("FalconApp", MarkCutElems);

InputParameters
MarkCutElems::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("Marks elements cut by mesh.");
  params.addRequiredParam<MeshFileName>("mesh_file", "Intersecting mesh file.");
  return params;
}

MarkCutElems::MarkCutElems(const InputParameters &parameters) : AuxKernel(parameters), _cutter_mesh(loadCutterMesh(getParam<MeshFileName>("mesh_file")))
{
  if (isNodal())
    paramError("variable", "The variable must be elemental");
  if (_mesh.dimension() != 3)
    mooseError("The mesh dimension must be 3D");

  buildCutterBoundingBoxes();
}

std::unique_ptr<const ReplicatedMesh> MarkCutElems::loadCutterMesh(const MeshFileName &filename) const
{
  // Load the mesh from file
  std::unique_ptr<ReplicatedMesh> mesh = std::make_unique<ReplicatedMesh>(_communicator);
  mesh->read(filename);

  // Make sure it's 2D TRI3 elems
  for (const auto &elem : mesh->element_ptr_range())
    if (elem->type() != TRI3)
      paramError("mesh_file", "The provided mesh does not contain all TRI3 elements");

  // This lets us return a const mesh
  return std::unique_ptr<const ReplicatedMesh>(std::move(mesh));
}

void MarkCutElems::buildCutterBoundingBoxes()
{
  _cutter_bboxes.clear();

  // Get the bounding box of this processor
  const auto pid_bbox = MeshTools::create_local_bounding_box(_mesh.getMesh());

  // Build the list of cut elems that may intersect this processor,
  // and store their bounding boxes
  for (const auto &elem : _cutter_mesh->element_ptr_range())
  {
    auto bbox = elem->loose_bounding_box();
    bbox.scale(1.01); // scale by a little for wiggle room in comparison
    if (pid_bbox.intersects(bbox))
      _cutter_bboxes.emplace_back(elem, bbox);
  }
}

void MarkCutElems::meshChanged()
{
  buildCutterBoundingBoxes();
}

Real MarkCutElems::computeValue()
{
  // The bounding box of the current element
  const auto bbox = _current_elem->loose_bounding_box();

  // Temproraries for doing things below
  std::unique_ptr<const Elem> edge;
  Real intersection_distance;
  ElemExtrema intersected_extrema;

  // Check intersection with each cut elem that intersects our local bbox
  for (const auto &[cut_elem, cut_bbox] : _cutter_bboxes)
  {
    // Bounding box of the cut elem doesn't intersect the current elem bbox
    // ...nothing to do here!
    if (!cut_bbox.intersects(bbox))
      continue;

    // Check for an intersection with each edge and the cut tri
    // We have to check the intersection with each orientation of the edge
    for (const auto e : _current_elem->edge_index_range())
    {
      _current_elem->build_edge_ptr(edge, e);

      const auto &edge0 = edge->point(0);
      const auto &edge1 = edge->point(1);
      const auto edge_length = edge->volume();
      const auto direction = (edge0 - edge1) / edge_length;

      // Check for an intersection
      // We need to check both "directions" of the edge due to the
      // intersection algorithm
      if ((TraceRayTools::intersectTriangle(edge1,
                                            direction,
                                            cut_elem,
                                            0,
                                            1,
                                            2,
                                            intersection_distance,
                                            intersected_extrema,
                                            /* hmax = */ 1.) ||
           TraceRayTools::intersectTriangle(edge0,
                                            -direction,
                                            cut_elem,
                                            0,
                                            1,
                                            2,
                                            intersection_distance,
                                            intersected_extrema,
                                            /* hmax = */ 1.)) &&
          intersection_distance <= edge_length)

        return 1;
    }
  }

  return 0;
}
