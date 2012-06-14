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

#ifndef STOCHASTICPOROUSMEDIA_H
#define STOCHASTICPOROUSMEDIA_H

#include "Material.h"


//Forward Declarations
class StochasticPorousMedia;

template<>
InputParameters validParams<StochasticPorousMedia>();

/**
 * Simple material with StochasticPorousMedia properties.
 */
class StochasticPorousMedia : public Material
{
public:
  StochasticPorousMedia(const std::string & name,
                        InputParameters parameters);
  
protected:
  virtual void computeProperties();


  
//  Real _input_permeability;

  bool _has_permeability;
  VariableValue & _init_permeability;
  Real _input_material_porosity;  //this is what get out into the material kernel input
  Real _input_density_rock;
  Real _input_compressibility;  

  Real _input_gravity;
  Real _gx;
  Real _gy;
  Real _gz;


  MaterialProperty<Real> & _permeability;
  MaterialProperty<Real> & _material_porosity;
  MaterialProperty<Real> & _density_rock;
  MaterialProperty<Real> & _compressibility;
  MaterialProperty<Real> & _gravity;
  MaterialProperty<RealVectorValue> & _gravity_vector;
  bool _has_frac_perm; 
  VariableValue & _z_disp;
  VariableValue & _z_disp_old;
  //MaterialProperty<Real> & _permeability_old;
  MaterialProperty<Real> & _aperture;
  MaterialProperty<RealVectorValue> & _strain;   
};


#endif //STOCHASTICPOROUSMEDIA_H
