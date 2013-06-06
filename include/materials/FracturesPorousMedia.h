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

  Real _fracture_permeability;
  Real _fracture_porosity;
  Real _fracture_density;

  Real _matrix_permeability;
  Real _matrix_porosity;
  Real _matrix_density;

  Real _input_gravity;
  Real _gx;
  Real _gy;
  Real _gz;
  
  MaterialProperty<Real> & _permeability;
  MaterialProperty<Real> & _material_porosity;
  MaterialProperty<Real> & _density_rock;
  MaterialProperty<Real> & _gravity;
  MaterialProperty<RealVectorValue> & _gravity_vector;
    
  bool _has_fractures;
  VariableValue & _fractures;
  Real _fracture_num;
  Real _matrix_num;
  Real _model_fracture_aperture;

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
