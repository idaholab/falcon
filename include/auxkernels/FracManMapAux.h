/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef FRACMANMAPAUX_H
#define FRACMANMAPAUX_H

#include "AuxKernel.h"
#include "libmesh/plane.h"
#include "libmesh/point.h"
#include "MooseEnum.h"
#include <string>

class FracManMapAux;

template<>
InputParameters validParams<FracManMapAux>();


/**
 * Load FracMan DFN files so that
 * the fractue location data can be incorporated into the mesh.
 */
class FracManMapAux : public AuxKernel
{
public:

  FracManMapAux(const InputParameters & parameters);
  virtual ~FracManMapAux() {}

protected:

  virtual Real computeValue();

  bool _fracture_rewrite;

  MooseEnum _metric_conversion;
  MooseEnum _normal_component;
  MooseEnum _output_type;

  Real dfn_box_x_max;
  Real dfn_box_y_max;
  Real dfn_box_z_max;
  Real dfn_box_x_min;
  Real dfn_box_y_min;
  Real dfn_box_z_min;
  Real wellbore_box_x_min;
  Real wellbore_box_y_min;
  Real wellbore_box_x_max;
  Real wellbore_box_y_max;

  std::vector<int> _fracture_number_vec;

  std::vector<Real> _coordinate_shift;
  std::vector<Real> _dfn_box_max_adjustment;
  std::vector<Real> _dfn_box_min_adjustment;
  std::vector<Real> _dfn_offset;
  std::vector<Real> _dfn_size;
  std::vector<Real> _dfn_tol;
  std::vector<Real> fracture_normal_x;
  std::vector<Real> fracture_normal_y;
  std::vector<Real> fracture_normal_z;
  std::vector<Real> fracture_vertices_x;
  std::vector<Real> fracture_vertices_y;
  std::vector<Real> fracture_vertices_z;
  std::vector<Real> _mesh_size;
  std::vector<Real> _refine_schedule;
  std::vector<Real> _fracture_tol;
  std::vector<Real> _wellbore_location;
  std::vector<Real> _wellbore_tol;

  std::vector<std::string> _file_names;

  unsigned int _nstages;
  unsigned int num_vec_entries;
};

#endif
