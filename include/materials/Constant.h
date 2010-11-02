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
  Constant(const std::string & name,
           MooseSystem & moose_system,
           InputParameters parameters);
  
protected:
  virtual void computeProperties();
  
private:

  bool _has_temp;
  VariableValue & _temp;

  Real _my_thermal_conductivity;
  Real _my_thermal_expansion;
  Real _my_specific_heat;
  Real _my_density;
  Real _my_youngs_modulus;
  Real _my_poissons_ratio;
  Real _my_biot_coeff;  
  Real _my_t_ref;

  MaterialProperty<Real> & _thermal_conductivity;
  MaterialProperty<Real> & _thermal_strain;
  MaterialProperty<Real> & _alpha;
  MaterialProperty<Real> & _biot_coeff;
  MaterialProperty<Real> & _specific_heat;
  MaterialProperty<Real> & _density;
  MaterialProperty<Real> & _rho_r;
  MaterialProperty<Real> & _youngs_modulus;
  MaterialProperty<Real> & _poissons_ratio;
};

#endif //CONSTANT_H
