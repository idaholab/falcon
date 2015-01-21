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

//Forward Declarations
class FracManMapAux;

template<>
InputParameters validParams<FracManMapAux>();

/**
 * Coupled auxiliary value
 */
class FracManMapAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all
   * derived classes can be built using the same
   * constructor.
   */
  FracManMapAux(const std::string & name, InputParameters parameters);

  virtual ~FracManMapAux() {}

protected:
  virtual Real computeValue();

    std::vector<Real> _coordinate_shift;
    Real dfn_box_x_max, dfn_box_y_max, dfn_box_z_max;
    Real dfn_box_x_min, dfn_box_y_min, dfn_box_z_min;
    std::vector<Real> _dfn_dimension;
    std::vector<Real> _dfn_tol;
    std::vector<std::string> _file_names;
    std::vector<Real> fracture_normal_x;
    std::vector<Real> fracture_normal_y;
    std::vector<Real> fracture_normal_z;
    std::vector<int> _fracture_number_vec;
    bool _fracture_rewrite;
    std::vector<Real> fracture_vertices_x;
    std::vector<Real> fracture_vertices_y;
    std::vector<Real> fracture_vertices_z;
    MooseEnum _metric_conversion;
    std::vector<Real> _mesh_dimension;
    unsigned int _nstages;
    MooseEnum _normal_component;
    unsigned int num_vec_entries;
    MooseEnum _output_type;
    std::vector<Real> _refine_schedule;
    std::vector<Real> _fracture_tol;
    Real wellbore_box_x_min, wellbore_box_y_min;
    Real wellbore_box_x_max, wellbore_box_y_max;
    std::vector<Real> _wellbore_location;
    std::vector<Real> _wellbore_tol;

};

#endif //FRACMANMAPAUX_H
