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
             InputParameters parameters);

protected:
  virtual void computeProperties();

private:
  Real _input_permeability;
  Real _input_porosity;
  Real _input_density_water;  
  Real _input_viscosity_water;
  Real _input_compressibility;  
  Real _input_thermal_conductivity;
  Real _input_time_coefficient;
  Real _input_water_specif_heat;
  Real _input_specific_heat_rock;
  Real _input_density_rock;
  Real _input_gravity;
  Real _gx;
  Real _gy;
  Real _gz;  

  MaterialProperty<Real> & _permeability;
  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _density_water;
  MaterialProperty<Real> & _viscosity_water;
  MaterialProperty<Real> & _compressibility;
  MaterialProperty<Real> & _thermal_conductivity;
  MaterialProperty<Real> & _time_coefficient;
  MaterialProperty<Real> & _gravity;
  MaterialProperty<Real> & _specific_heat_water;
  MaterialProperty<Real> & _specific_heat_rock;
  MaterialProperty<Real> & _density_rock;
  MaterialProperty<Real> & _tau_water;
  MaterialProperty<RealGradient> & _darcy_flux_water;
  MaterialProperty<RealGradient> & _darcy_velocity_w;
  MaterialProperty<RealVectorValue> & _gravity_vector;
  VariableGradient & _grad_p;
  VariableValue  & _pressure;
  VariableValue  & _temperature;
  VariableGradient & _grad_T;
  
};

#endif //DARCYWATER_H
