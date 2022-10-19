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

//! Authors: Yidong Xia (Yidong.Xia@inl.gov)
//! Created: 09/07/2014

#include "MarkCutElems.h"
#include "TraceRayTools.h"
#include "libmesh/enum_to_string.h"

registerMooseObject("FalconApp", MarkCutElems);

InputParameters
MarkCutElems::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("Marks elements cut by mesh.");
  params.addRequiredParam<MeshFileName>("mesh_file", "Intersecting mesh file.");
  return params;
}

MarkCutElems::MarkCutElems(const InputParameters & parameters) : AuxKernel(parameters)
{
  if (isNodal())
    mooseError("MarkCutElems only works on elemental fields.");

  MeshFileName xfem_cutter_mesh_file = getParam<MeshFileName>("mesh_file");
  _cutter_mesh = std::make_shared<ReplicatedMesh>(_communicator);
  _cutter_mesh->read(xfem_cutter_mesh_file);
  for (const auto & elem : _cutter_mesh->element_ptr_range())
    if (elem->type() != TRI3)
      mooseError("MarkCutElems currently only supports TRI3 elements in the "
                 "cutting mesh.");
}

Real
MarkCutElems::computeValue()
{
  mooseAssert(_current_elem->dim() == 3, "Dimension of element to be cut must be 3");

  for (unsigned int i = 0; i < _current_elem->n_sides(); ++i)
  {
    // This returns the lowest-order type of side.
    std::unique_ptr<const Elem> curr_side = _current_elem->side_ptr(i);

    mooseAssert(curr_side->dim() == 2, "Side dimension must be 2");

    unsigned int n_edges = curr_side->n_sides();

    for (unsigned int j = 0; j < n_edges; j++)
    {
      std::unique_ptr<const Elem> curr_edge = curr_side->side_ptr(j);
      if (curr_edge->type() != EDGE2)
        mooseError("In cutElementByGeometry face edge must be EDGE2, but type is: ",
                   libMesh::Utility::enum_to_string(curr_edge->type()),
                   " base element type is: ",
                   libMesh::Utility::enum_to_string(_current_elem->type()));
      const Node * node1 = curr_edge->node_ptr(0);
      const Node * node2 = curr_edge->node_ptr(1);
      Real edgeLength = curr_edge->volume();
      for (const auto & cut_elem : _cutter_mesh->element_ptr_range())
      {
        Real intersection_distance = -1;
        ElemExtrema intersected_extrema;
        const Real hmax = 1.0;
        bool intersects = TraceRayTools::intersectTriangle(*node1,
                                                           (*node2 - *node1) / edgeLength,
                                                           cut_elem,
                                                           (const unsigned short)0,
                                                           (const unsigned short)1,
                                                           (const unsigned short)2,
                                                           intersection_distance,
                                                           intersected_extrema,
                                                           hmax);

        if (intersects && intersection_distance < edgeLength)
          return 1;
      }
    }
  }
  return 0;
}
