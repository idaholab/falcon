#include "Material.h"

#ifndef STOCHASTICMATERIAL_H
#define STOCHASTICMATERIAL_H

//Forward Declarations
class StochasticMaterial;

template<>
InputParameters validParams<StochasticMaterial>();

/**
 * Example material class that defines a few properties.
 */
class StochasticMaterial : public Material
{
public:
  StochasticMaterial(const std::string & name,
                  InputParameters parameters);

protected:
  virtual void computeProperties();

private:
  Real _input_diffusivity;
  Real _input_initial_porosity;
  Real _input_initial_Ss;
  Real _input_rho_w;  
  Real _input_mu_w;
  Real _input_gravity;
  
  bool _has_cond;
  VariableValue & _init_cond;
  bool _has_aptr;
  VariableValue & _init_aptr;

  std::vector<Real> _molecular_weight;
  std::vector<Real> _mineral_density;
  std::vector<Real> _input_initial_mineral;
  

  MaterialProperty<Real> & _diffusivity;
  MaterialProperty<Real> & _Ss;
  MaterialProperty<Real> & _frac_stor;
  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _rho_w;
  MaterialProperty<Real> & _mu_w;
  MaterialProperty<Real> & _gravity;
  MaterialProperty<Real> & _frac_trnsm;
  MaterialProperty<Real> & _frac_cond;
  MaterialProperty<Real> & _cond;
  MaterialProperty<Real> & _aptr;

  std::vector<VariableValue *> _vals; //coupled to minerals

};

#endif //STOCHASTICMATERIAL_H
