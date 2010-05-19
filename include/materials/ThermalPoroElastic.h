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
  ThermalPoroElastic(std::string name,
                     MooseSystem & moose_system,
                     InputParameters parameters);
  
protected:
  virtual void computeProperties();

private:

  bool _has_temp;
  bool _has_variable_density;
  bool _has_pressure;
  bool _has_solid_mechanics;
  bool _has_x_disp;
  bool _has_y_disp;
  bool _has_z_disp;

  std::vector<Real> & _temperature;
  std::vector<RealGradient> & _grad_temp;
  std::vector<Real> & _pressure;
  std::vector<RealGradient> & _grad_p;

  std::vector<RealGradient> & _grad_x_disp;
  std::vector<RealGradient> & _grad_y_disp;
  std::vector<RealGradient> & _grad_z_disp;

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

  MooseArray<Real> & _permeability;
  MooseArray<Real> & _porosity;
  MooseArray<Real> & _rho_r;
  MooseArray<Real> & _rock_specific_heat;
  MooseArray<Real> & _thermal_conductivity;
  MooseArray<Real> & _thermal_strain;
  MooseArray<Real> & _alpha;
  MooseArray<Real> & _youngs_modulus;
  MooseArray<Real> & _poissons_ratio;  
  MooseArray<Real> & _biot_coeff;
  
  MooseArray<Real> & _rho_w;
  MooseArray<Real> & _mu_w;
  MooseArray<Real> & _c_f;
  MooseArray<Real> & _water_specific_heat;
  
  MooseArray<Real> & _darcy_params;
  MooseArray<RealGradient> & _darcy_flux;
  MooseArray<RealGradient> & _pore_velocity;
  
  MooseArray<Real> & _gravity;
  MooseArray<RealVectorValue> & _gravity_vector;
  MooseArray<RealVectorValue> & _stress_normal_vector;
  MooseArray<RealVectorValue> & _stress_shear_vector;
  MooseArray<RealVectorValue> & _strain_normal_vector;
  MooseArray<RealVectorValue> & _strain_shear_vector;  

};

#endif //THERMALPOROELASTIC_H
