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

  std::vector<Real> & _permeability;
  std::vector<Real> & _porosity;
  std::vector<Real> & _rho_w;
  std::vector<Real> & _mu_w;
  std::vector<Real> & _c_f;
  std::vector<Real> & _thermal_conductivity;
  std::vector<Real> & _time_coefficient;
  std::vector<Real> & _gravity;
  std::vector<Real> & _water_specific_heat;
  std::vector<Real> & _rock_specific_heat;
  std::vector<Real> & _rho_r;
  std::vector<Real> & _darcy_params;
  std::vector<RealGradient> & _darcy_flux;
  std::vector<RealGradient> & _darcy_velocity;
  std::vector<RealVectorValue> & _gravity_vector;
  std::vector<RealGradient> & _grad_p;
  std::vector<Real> & _pressure;
  std::vector<Real> & _temperature;
  std::vector<RealGradient> & _grad_T;
  
};

#endif //DARCYWATER_H
