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
           InputParameters parameters,
           unsigned int block_id,
           std::vector<std::string> coupled_to,
           std::vector<std::string> coupled_as);
  
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

  std::vector<Real> & _permeability;
  std::vector<Real> & _porosity;
  std::vector<Real> & _rho_r;
  std::vector<Real> & _rock_specific_heat;
  std::vector<Real> & _thermal_conductivity;
  std::vector<Real> & _thermal_strain;
  std::vector<Real> & _alpha;
  std::vector<Real> & _youngs_modulus;
  std::vector<Real> & _poissons_ratio;  
  std::vector<Real> & _biot_coeff;
  
  std::vector<Real> & _rho_w;
  std::vector<Real> & _mu_w;
  std::vector<Real> & _c_f;
  std::vector<Real> & _water_specific_heat;
  
  std::vector<Real> & _darcy_params;
  std::vector<RealGradient> & _darcy_flux;
  std::vector<RealGradient> & _pore_velocity;
  
  std::vector<Real> & _gravity;
  std::vector<RealVectorValue> & _gravity_vector;
  std::vector<RealVectorValue> & _stress_normal_vector;
  std::vector<RealVectorValue> & _stress_shear_vector;
  std::vector<RealVectorValue> & _strain_normal_vector;
  std::vector<RealVectorValue> & _strain_shear_vector;  

};

#endif //THERMALPOROELASTIC_H
