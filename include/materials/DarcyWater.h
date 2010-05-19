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
  DarcyWater(std::string name,
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

  MooseArray<Real> & _permeability;
  MooseArray<Real> & _porosity;
  MooseArray<Real> & _rho_w;
  MooseArray<Real> & _mu_w;
  MooseArray<Real> & _c_f;
  MooseArray<Real> & _thermal_conductivity;
  MooseArray<Real> & _time_coefficient;
  MooseArray<Real> & _gravity;
  MooseArray<Real> & _water_specific_heat;
  MooseArray<Real> & _rock_specific_heat;
  MooseArray<Real> & _rho_r;
  MooseArray<Real> & _darcy_params;
  MooseArray<RealGradient> & _darcy_flux;
  MooseArray<RealGradient> & _darcy_velocity;
  MooseArray<RealVectorValue> & _gravity_vector;
  std::vector<RealGradient> & _grad_p;
  std::vector<Real> & _pressure;
  std::vector<Real> & _temperature;
  std::vector<RealGradient> & _grad_T;
  
};

#endif //DARCYWATER_H
