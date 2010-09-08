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
class SolidMechanics : virtual public PorousMedia
{
public:
  SolidMechanics(const std::string & name,
                 MooseSystem & moose_system,
                 InputParameters parameters);
  
protected:
  virtual void computeProperties();

  Real _input_thermal_expansion;
  Real _input_youngs_modulus;
  Real _input_poissons_ratio;

  Real _input_biot_coeff;
  Real _input_t_ref;
  
  MaterialProperty<Real> & _thermal_strain;
  MaterialProperty<Real> & _alpha;
  MaterialProperty<Real> & _youngs_modulus;
  MaterialProperty<Real> & _poissons_ratio;
  MaterialProperty<Real> & _biot_coeff;

  MaterialProperty<RealVectorValue> & _stress_normal_vector;
  MaterialProperty<RealVectorValue> & _stress_shear_vector;
  MaterialProperty<RealVectorValue> & _strain_normal_vector;
  MaterialProperty<RealVectorValue> & _strain_shear_vector;
  
private:

  bool _has_temp;
  bool _has_x_disp;
  bool _has_y_disp;
  bool _has_z_disp;

  VariableValue  & _temperature;
  VariableGradient & _grad_temp;

  VariableGradient & _grad_x_disp;
  VariableGradient & _grad_y_disp;
  VariableGradient & _grad_z_disp;
  
  Real _E;
  Real _nu;
  Real _c1;
  Real _c2;
  Real _c3;
};

#endif //SOLIDMECHANICS_H
