//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "RemoveDuplicateFacesFromSidesets.h"

#include "CastUniquePointer.h"

#include "libmesh/boundary_info.h"
#include "libmesh/fe_base.h"
#include "libmesh/quadrature_gauss.h"
#include "libmesh/string_to_enum.h"

registerMooseObject("MooseApp", RemoveDuplicateFacesFromSidesets);

InputParameters
RemoveDuplicateFacesFromSidesets::validParams()
{
  InputParameters params = MeshGenerator::validParams();

  params.addClassDescription("Mesh generator which removes duplicate sidesets "
                             "from meshes created with a gmsh script for");
  params.addRequiredParam<MeshGeneratorName>("input", "The mesh we want to modify");

  return params;
}

RemoveDuplicateFacesFromSidesets::RemoveDuplicateFacesFromSidesets(
    const InputParameters & parameters)
  : MeshGenerator(parameters), _input(getMesh("input"))
{
}

std::unique_ptr<MeshBase>
RemoveDuplicateFacesFromSidesets::generate()
{

  std::unique_ptr<MeshBase> mesh = std::move(_input);

  // To know the dimension of the mesh
  if (!mesh->is_prepared())
    mesh->prepare_for_use();
  const auto dim = mesh->mesh_dimension();

  // Setup the FE Object so we can calculate normals
  FEType fe_type(Utility::string_to_enum<Order>("CONSTANT"),
                 Utility::string_to_enum<FEFamily>("MONOMIAL"));

  std::unique_ptr<libMesh::FEBase> fe_face = libMesh::FEBase::build(dim, fe_type);

  std::unique_ptr<libMesh::QGauss> qface = std::make_unique<libMesh::QGauss>(dim - 1, FIRST);

  fe_face->attach_quadrature_rule(qface.get());
  // Must always pre-request quantities you want to compute
  fe_face->get_normals();

  // Get the current list of boundary map so we can delete some of
  // them
  BoundaryInfo & boundary_info = mesh->get_boundary_info();
  const auto sideset_map = boundary_info.get_sideset_map();

  std::vector<std::pair<dof_id_type, std::pair<unsigned short int, boundary_id_type>>>
      sides_to_remove;

  for (const auto & [elem, id_pair] : sideset_map)
  {
    const auto side_id = id_pair.first;
    const auto sideset_id = id_pair.second;
    // ignore boundary elements
    const Elem * neighbor = elem->neighbor_ptr(side_id);
    if (!neighbor)
      continue;

    // this side does not have a duplicate
    if (boundary_info.side_with_boundary_id(neighbor, sideset_id) == libMesh::invalid_uint)
      continue;

    fe_face->reinit(elem, side_id);
    // We'll just use the normal of the first qp
    const Point face_normal = fe_face->get_normals()[0];

    if (face_normal(0) < 0 || (face_normal(0) == 0 && face_normal(1) < 0) ||
        (face_normal(0) == 0 && face_normal(2) < 0))
    {
      dof_id_type elem_id = elem->id();
      sides_to_remove.push_back(std::make_pair(elem_id, id_pair));
    }
  }

  for (const auto & [elem_id, id_pair] : sides_to_remove)
  {
    const auto elem_pointer = mesh->elem_ptr(elem_id);
    const auto side_id = id_pair.first;
    const auto sideset_id = id_pair.second;
    boundary_info.remove_side(elem_pointer, side_id, sideset_id);
  }

  mesh->set_isnt_prepared();
  return dynamic_pointer_cast<MeshBase>(mesh);
}
