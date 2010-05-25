#ifndef CONSTANT_H
#define CONSTANT_H

#include "Material.h"


//Forward Declarations
class Constant;

template<>
InputParameters validParams<Constant>();

/**
 * Simple material with constant properties.
 */
class Constant : public Material
{
public:
  Constant(std::string name,
           MooseSystem & moose_system,
           InputParameters parameters);
  
protected:
  virtual void computeProperties();
  
private:

  bool _has_temp;
  MooseArray<Real> & _temp;

  Real _my_thermal_conductivity;
  Real _my_thermal_expansion;
  Real _my_biot_coeff;  
  Real _my_specific_heat;
  Real _my_density;
  Real _my_youngs_modulus;
  Real _my_poissons_ratio;
  Real _my_t_ref;

  MooseArray<Real> & _thermal_conductivity;
  MooseArray<Real> & _thermal_strain;
  MooseArray<Real> & _alpha;
  MooseArray<Real> & _biot_coeff;
  MooseArray<Real> & _specific_heat;
  MooseArray<Real> & _density;
  MooseArray<Real> & _rho_r;
  MooseArray<Real> & _youngs_modulus;
  MooseArray<Real> & _poissons_ratio;
};

#endif //CONSTANT_H
