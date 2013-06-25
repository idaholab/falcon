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

#ifndef FRACTURESPOROUSMEDIA_H
#define FRACTURESPOROUSMEDIA_H

#include "Material.h"


//Forward Declarations
class FracturesPorousMedia;

template<>
InputParameters validParams<FracturesPorousMedia>();

/**
 * Simple material with PorousMedia properties.
 */
class FracturesPorousMedia : public Material
{
public:
  FracturesPorousMedia(const std::string & name,
              InputParameters parameters);
  
protected:
  virtual void computeProperties();
////Grab user input parameters
  ////Matrix
  //rock property inputs
  Real _matrix_permeability;
  Real _matrix_porosity;
  Real _matrix_density;
  //chem reaction inputs
  Real _matrix_chem_diff;
  std::vector<Real> _matrix_mineral;
  std::vector<Real> _matrix_molecular_weight;
  std::vector<Real> _matrix_mineral_density;
  
  ////Gravity
  Real _input_gravity;
  Real _gx;
  Real _gy;
  Real _gz;
    
  ////Fractures
  //fracture map inputs
  bool _has_fractures;
  VariableValue & _fractures;
  Real _fracture_num;
  Real _matrix_num;
  Real _model_fracture_aperture;
  //rock property inputs
  Real _fracture_permeability;
  Real _fracture_porosity;
  Real _fracture_density;
  //chem reaction inputs
  Real _fracture_chem_diff;
  std::vector<Real> _fracture_mineral;
  std::vector<Real> _fracture_molecular_weight;
  std::vector<Real> _fracture_mineral_density;
  bool _has_chem_reactions;
  std::vector<VariableValue *> _vals;
  
////Delcare material properties
  //rock material props
  MaterialProperty<Real> & _permeability;
  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _density_rock;
  //gravity material props
  MaterialProperty<Real> & _gravity;
  MaterialProperty<RealVectorValue> & _gravity_vector;
  //chem reactions material props
  MaterialProperty<Real> & _diffusivity;

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


#endif //FRACTURESPOROUSMEDIA_H
