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
  Real _input_porosity;
  Real _input_density_rock;

  Real _input_gravity;
  Real _gx;
  Real _gy;
  Real _gz;

  MaterialProperty<Real> & _permeability;
  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _density_rock;
  
  MaterialProperty<Real> & _gravity;
  MaterialProperty<RealVectorValue> & _gravity_vector;

};

#endif //POROUSMEDIA_H
