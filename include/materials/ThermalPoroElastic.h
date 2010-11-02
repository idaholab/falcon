#ifndef THERMALPOROELASTIC_H
#define THERMALPOROELASTIC_H

#include "Material.h"


//Forward Declarations
class ThermalPoroElastic;

template<>
InputParameters validParams<ThermalPoroElastic>();

/**
 * Simple material with ThermalPoroElastic properties.
 */
class ThermalPoroElastic : public Material
{
public:
  ThermalPoroElastic(const std::string & name,
                     MooseSystem & moose_system,
                     InputParameters parameters);
  
protected:
  virtual void computeProperties();

private:
  bool _has_pressure;
  VariableGradient & _grad_p;
  VariableValue  & _pressure;

  bool _has_temp;
  VariableGradient & _grad_temp;
  VariableValue  & _temperature;

  bool _has_x_disp;
  VariableGradient & _grad_x_disp;
  bool _has_y_disp;
  VariableGradient & _grad_y_disp;
  bool _has_z_disp;
  VariableGradient & _grad_z_disp;

  bool _has_solid_mechanics;
  bool _has_variable_density;



  Real _input_permeability;
  Real _input_porosity;
  Real _input_rho_r;
  Real _input_rock_specific_heat;
  Real _input_thermal_conductivity;
  Real _input_thermal_expansion;
  Real _input_youngs_modulus;
  Real _input_poissons_ratio;
  Real _input_biot_coeff;
  Real _input_t_ref;
  
  Real _input_rho_w;
  Real _input_mu_w;
  Real _input_c_f;
  Real _input_water_specific_heat;

  Real _input_gravity;
  Real _gx;
  Real _gy;
  Real _gz;

  Real _E;
  Real _nu;
  Real _c1;
  Real _c2;
  Real _c3;

  MaterialProperty<Real> & _permeability;
  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _rho_r;
  MaterialProperty<Real> & _rock_specific_heat;
  MaterialProperty<Real> & _thermal_conductivity;
  MaterialProperty<Real> & _thermal_strain;
  MaterialProperty<Real> & _alpha;
  MaterialProperty<Real> & _youngs_modulus;
  MaterialProperty<Real> & _poissons_ratio;
  MaterialProperty<Real> & _biot_coeff;
  
  MaterialProperty<Real> & _rho_w;
  MaterialProperty<Real> & _mu_w;
  MaterialProperty<Real> & _c_f;
  MaterialProperty<Real> & _water_specific_heat;
  
  MaterialProperty<Real> & _darcy_params_w;
  MaterialProperty<RealGradient> & _darcy_flux_w;
  MaterialProperty<RealGradient> & _pore_velocity_w;
  
  MaterialProperty<Real> & _gravity;
  MaterialProperty<RealVectorValue> & _gravity_vector;
  MaterialProperty<RealVectorValue> & _stress_normal_vector;
  MaterialProperty<RealVectorValue> & _stress_shear_vector;
  MaterialProperty<RealVectorValue> & _strain_normal_vector;
  MaterialProperty<RealVectorValue> & _strain_shear_vector;

};

#endif //THERMALPOROELASTIC_H
