#include "AnalyticalADE1D.h"

template<>
InputParameters validParams<AnalyticalADE1D>()
{
  InputParameters params;
  return params;
}

AnalyticalADE1D::AnalyticalADE1D(std::string name,
                         InputParameters parameters,
                         std::string var_name,
                         std::vector<std::string> coupled_to,
                         std::vector<std::string> coupled_as)
  :AuxKernel(name, parameters, var_name, coupled_to, coupled_as)
{}


Real
AnalyticalADE1D::computeValue()
{

  Real _ro_r = 2.50e3;
  Real _c_r = 0.92e3;
  Real _thermal_conductivity = 1.5;
  Real _n = 0.20;

  Real _ro_w = 1.0e3;
  Real _c_w = 4.186e3;
  Real _v = 5.0e-6;

  Real _thermal_diffusivity = _thermal_conductivity/(_n*_ro_w*_c_w+(1.0-_n)*_ro_r*_c_r);
  Real _vel = _n*_ro_w*_c_w/(_n*_ro_w*_c_w+(1.0-_n)*_ro_r*_c_r)*_v;

  Real var1 = -((*_current_node)(0)-_vel * _t)/2.0/std::sqrt(_thermal_diffusivity * _t); 

  return 100.0-100.0*0.5*(1.0 - erf(var1));
  
  
}
