#include "Material.h"

#ifndef DARCYWATER_H
#define DARCYWATER_H

//Forward Declarations
class DarcyWater;

template<>
InputParameters validParams<DarcyWater>();

class DarcyWater : public Material
{
public:
  DarcyWater(const std::string & name,
             MooseSystem & moose_system,
             InputParameters parameters);

protected:
  virtual void computeProperties();

private:
  Real _input_permeability;
  Real _input_porosity;
  Real _input_rho_w;  
  Real _input_mu_w;
  Real _input_c_f;  
  Real _input_thermal_conductivity;
  Real _input_time_coefficient;
  Real _input_water_specif_heat;
  Real _input_rock_specific_heat;
  Real _input_rho_r;
  Real _input_gravity;
  Real _gx;
  Real _gy;
  Real _gz;  

  MaterialProperty<Real> & _permeability;
  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _rho_w;
  MaterialProperty<Real> & _mu_w;
  MaterialProperty<Real> & _c_f;
  MaterialProperty<Real> & _thermal_conductivity;
  MaterialProperty<Real> & _time_coefficient;
  MaterialProperty<Real> & _gravity;
  MaterialProperty<Real> & _water_specific_heat;
  MaterialProperty<Real> & _rock_specific_heat;
  MaterialProperty<Real> & _rho_r;
  MaterialProperty<Real> & _darcy_params_w;
  MaterialProperty<RealGradient> & _darcy_flux_w;
  MaterialProperty<RealGradient> & _darcy_velocity_w;
  MaterialProperty<RealVectorValue> & _gravity_vector;
  VariableGradient & _grad_p;
  VariableValue  & _pressure;
  VariableValue  & _temperature;
  VariableGradient & _grad_T;
  
};

#endif //DARCYWATER_H
