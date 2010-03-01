#ifndef SOLIDMECHANICS_H
#define SOLIDMECHANICS_H

#include "PorousMedia.h"


//Forward Declarations
class SolidMechanics;

template<>
InputParameters validParams<SolidMechanics>();

/**
 * Simple material with SolidMechanics properties.
 */
class SolidMechanics : public PorousMedia
{
public:
  SolidMechanics(std::string name,
           InputParameters parameters,
           unsigned int block_id,
           std::vector<std::string> coupled_to,
           std::vector<std::string> coupled_as);
  
protected:
  virtual void computeProperties();

  Real _input_thermal_expansion;
  Real _input_youngs_modulus;
  Real _input_poissons_ratio;

  Real _input_biot_coeff;
  Real _input_t_ref;
  
  std::vector<Real> & _thermal_strain;
  std::vector<Real> & _alpha;
  std::vector<Real> & _youngs_modulus;
  std::vector<Real> & _poissons_ratio;  
  std::vector<Real> & _biot_coeff;

  std::vector<RealVectorValue> & _stress_normal_vector;
  std::vector<RealVectorValue> & _stress_shear_vector;
  std::vector<RealVectorValue> & _strain_normal_vector;
  std::vector<RealVectorValue> & _strain_shear_vector;
  
private:

  bool _has_temp;
  bool _has_x_disp;
  bool _has_y_disp;
  bool _has_z_disp;

  std::vector<Real> & _temperature;
  std::vector<RealGradient> & _grad_temp;

  std::vector<RealGradient> & _grad_x_disp;
  std::vector<RealGradient> & _grad_y_disp;
  std::vector<RealGradient> & _grad_z_disp;
  
  Real _E;
  Real _nu;
  Real _c1;
  Real _c2;
  Real _c3;
};

#endif //SOLIDMECHANICS_H
