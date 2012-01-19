#ifndef POROUSMEDIA_H
#define POROUSMEDIA_H

#include "Material.h"


//Forward Declarations
class PorousMedia;

template<>
InputParameters validParams<PorousMedia>();

/**
 * Simple material with PorousMedia properties.
 */
class PorousMedia : public Material
{
public:
  PorousMedia(const std::string & name,
              InputParameters parameters);
  
protected:
  virtual void computeProperties();

  Real _input_permeability;
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
  MaterialProperty<Real> & _permeability_old;
  MaterialProperty<Real> & _aperture;
 // MaterialProperty<RealVectorValue> & _strain;   
};


#endif //POROUSMEDIA_H
