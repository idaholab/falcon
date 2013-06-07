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

  Real _input_density_rock;

  Real _matrix_permeability;
  Real _matrix_porosity;

  Real _input_gravity;
  Real _gx;
  Real _gy;
  Real _gz;

  MaterialProperty<Real> & _permeability;
  MaterialProperty<Real> & _material_porosity;
  MaterialProperty<Real> & _density_rock;
  MaterialProperty<Real> & _gravity;
  MaterialProperty<RealVectorValue> & _gravity_vector;

    std::vector<int> _fracture_number_vec;
    std::vector<Real> _fracture_permeability_vec;
    std::vector<Real> _fracture_porosity_vec;
    unsigned int num_frac_vec_entries;
    unsigned int num_perm_vec_entries;
    unsigned int num_poro_vec_entries;
    VariableValue & _fracture_map;

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
