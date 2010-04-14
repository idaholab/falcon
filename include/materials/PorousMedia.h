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
  PorousMedia(std::string name,
              MooseSystem & moose_system,
              InputParameters parameters);
  
protected:
  virtual void computeProperties();

  Real _input_permeability;
  Real _input_porosity;
  Real _input_rho_r;

  Real _input_gravity;
  Real _gx;
  Real _gy;
  Real _gz;

  std::vector<Real> & _permeability;
  std::vector<Real> & _porosity;
  std::vector<Real> & _rho_r;
  
  std::vector<Real> & _gravity;
  std::vector<RealVectorValue> & _gravity_vector;

};

#endif //POROUSMEDIA_H
