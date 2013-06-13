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
  
    std::vector<std::string> _file_names;
    std::vector<Real> fracture_normal_x;
    std::vector<Real> fracture_normal_y;
    std::vector<Real> fracture_normal_z;
    std::vector<int> _fracture_number_vec;
    std::vector<Real> fracture_vertices_x;
    std::vector<Real> fracture_vertices_y;
    std::vector<Real> fracture_vertices_z;
    MooseEnum _normal_component;
    unsigned int num_vec_entries;
    MooseEnum _output_type;
    Real _tol;
    
    /*Real _frac_man_map;
    std::string _file_name;
    //std::string _west_reduced_file_name;
    //std::string _fault_file_name;
    
    std::vector<int> _fracture_number_vec;
    unsigned int num_vec_entries;
    Real _tol;
    std::string buffer;
    Plane frac;
    Real frac_array[219][6];
    std::vector<int> frac_plane_vector;
    int frac_face_num = 0;
    int frac_file_code = 1;
    int frac_num = 0;
    Real frac_prop1 = 0;
    Real frac_prop2 = 0;
    Real frac_prop3 = 0;
    int frac_set_num = 0;
    int frac_total = 15;
    int frac_vert = 0;
    int frac_vert_num = 0;
    int i = 0;
    unsigned int j = 0;
    int k = 0;
    int line_num = 0;
    int m = 0;
    int vert_count = 0;
    bool vert_end = false;
    std::string vert_end_line;
    bool vert_start = false;
    std::string vert_start_line;
    int vert_start_line_num = 0;
    Real x = 0, y = 0, z = 0;
    Real xp = 0, yp = 0, zp = 0;
     */
};

#endif //FACMANMAPAUX_H
