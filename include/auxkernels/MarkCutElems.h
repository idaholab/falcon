//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "AuxKernel.h"

#include "libmesh/replicated_mesh.h"
#include "libmesh/bounding_box.h"

/**
 * Find 3D elements cut by lower dimensional mesh
 */
class MarkCutElems : public AuxKernel
{
public:
  static InputParameters validParams();
  MarkCutElems(const InputParameters &parameters);

  virtual void meshChanged() override;

protected:
  virtual Real computeValue() override;

private:
  /// Loads the mesh used as the cutter mesh with the filename \p filename
  std::unique_ptr<const ReplicatedMesh> loadCutterMesh(const MeshFileName &filename) const;

  /// Builds _cutter_bboxes
  void buildCutterBoundingBoxes();

  /// The mesh we want to cut with
  const std::unique_ptr<const ReplicatedMesh> _cutter_mesh;

  /// The elem:bbox pairs from the cut mesh that intersect our processor
  std::vector<std::pair<const Elem *, BoundingBox>> _cutter_bboxes;
};
