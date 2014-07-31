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

#ifndef FRACMANPOROUSMEDIA_H
#define FRACMANPOROUSMEDIA_H

#include "Material.h"


//Forward Declarations
class FracManPorousMedia;

template<>
InputParameters validParams<FracManPorousMedia>();

/**
 * Simple material with PorousMedia properties.
 */
class FracManPorousMedia : public Material
{
public:
  FracManPorousMedia(const std::string & name,
              InputParameters parameters);
  
protected:
  virtual void computeProperties();
////Grab user input parameters
  ////Matrix
  Real _matrix_permeability;
  Real _matrix_porosity;
  Real _matrix_density;
  Real _matrix_compressibility;

  ////Gravity
  Real _input_gravity;
  Real _gx;
  Real _gy;
  Real _gz;
    
  ////Fractures
  //fracture map inputs
  VariableValue & _fracture_map;
  std::vector<int> _fracture_number_vec;
  std::vector<Real> _model_fracture_aperture_vec;
  //rock property inputs
  std::vector<Real> _fracture_permeability_vec;
  std::vector<Real> _fracture_porosity_vec;
  std::vector<Real> _fracture_density_vec;
  std::vector<Real> _fracture_compressibility_vec;

  ////Storing the number of vector entries into respective local variables
  unsigned int num_frac_vec_entries;
  unsigned int num_aper_vec_entries;
  unsigned int num_perm_vec_entries;
  unsigned int num_poro_vec_entries;
  unsigned int num_dens_vec_entries;
  unsigned int num_comp_vec_entries;

  ////Delcare material properties
  //rock material props
  MaterialProperty<Real> & _permeability;
  bool _has_pressure_dependent_permeability;
  bool _has_strain_dependent_permeability;
  MaterialProperty<Real> *_permeability_old;
  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _density_rock;
  MaterialProperty<Real> & _compressibility;
  
  //gravity material props
  MaterialProperty<Real> & _gravity;
  MaterialProperty<RealVectorValue> & _gravity_vector;

  void setPropsComputed(bool value) { _already_computed = value; }
  
  bool areParentPropsComputed() const { return _already_computed; }
  
private:

  /**
   * This parameter is here to indicate whether or not the PorousMedia Material
   * properties have been computed.  Each of the classes in the diamond hierarchy
   * should query this variable (using the accessor) to determine whether or
   * not they should recompute the base class properties
   */
  bool _already_computed;
};


#endif //FRACMANPOROUSMEDIA_H
