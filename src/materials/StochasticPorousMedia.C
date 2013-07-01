/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "StochasticPorousMedia.h"
#include "Transient.h"

template<>
InputParameters validParams<StochasticPorousMedia>()
{
  InputParameters params = validParams<Material>();
  //rock property inputs
  params.addRequiredCoupledVar("permeability", "intrinsic permeability in m^2");
  params.addParam<Real>("porosity", 0.3, "rock porosity");
  params.addParam<Real>("density_rock", 2.50e3, "rock density, [kg/m^3]");
  //gravity inputs
  params.addParam<Real>("gravity",9.80665,"gravity acceleration constant, [m/s^2]");
  params.addParam<Real>("gx",0.0,"x component of the gravity pressure vector");
  params.addParam<Real>("gy",0.0,"y component of the gravity pressure vector");
  params.addParam<Real>("gz",1.0,"z component of the gravity pressure vector");
  //chemical reaction coupled variables and paramerters
  params.addParam<bool>("has_chem_reactions", false, "flag true if chemical reactions are present");
  params.addParam<Real>("diffusivity", 1e-8, "the chemical diffusicity, [m^2/s]");
  params.addParam<std::vector<Real> >("mineral", std::vector<Real>(1, 16.65), "Initial mineral concentration, [mol/L] solution");
  params.addParam<std::vector<Real> >("molecular_weight", std::vector<Real>(1, 100.08), "The molecular weight of mineral, [g/mol]");
  params.addParam<std::vector<Real> >("mineral_density", std::vector<Real>(1, 2.5), "The density of mineral [g/cm^3]");
  params.addCoupledVar("v", "caco3");
    
  return params;
}

StochasticPorousMedia::StochasticPorousMedia(const std::string & name,
                         InputParameters parameters)
  :Material(name, parameters),
////Grab user input parameters
    //rock property inputs
    _has_permeability(isCoupled("permeability")),
    _init_permeability(_has_permeability ? coupledValue("permeability") : _zero),
    _input_porosity(getParam<Real>("porosity")),
    _input_density_rock(getParam<Real>("density_rock")),
    //gravity inputs
    _input_gravity(getParam<Real>("gravity")),
    _gx(getParam<Real>("gx")),
    _gy(getParam<Real>("gy")),
    _gz(getParam<Real>("gz")),
    //chemical reaction inputs
    _has_chem_reactions(getParam<bool>("has_chem_reactions")),
    _input_chem_diff(getParam<Real>("diffusivity")),
    _mineral(getParam<std::vector<Real> >("mineral")),
    _molecular_weight(getParam<std::vector<Real> >("molecular_weight")),
    _mineral_density(getParam<std::vector<Real> >("mineral_density")),

////Delcare material properties
    //rock material props
    _permeability(declareProperty<Real>("permeability")),
    _porosity(declareProperty<Real>("porosity")),
    _density_rock(declareProperty<Real>("density_rock")),
    //gravity material props
    _gravity(declareProperty<Real>("gravity")),
    _gravity_vector(declareProperty<RealVectorValue>("gravity_vector")),
    //chem reactions material props
    _diffusivity(declareProperty<Real>("diffusivity")),

   _already_computed(false)

{
    //resize and fill in _vals with number of provided chem species
    int n = coupledComponents("v");
    _vals.resize(n);
    for (unsigned int i=0; i<_vals.size(); ++i)
        _vals[i] = &coupledValue("v", i);
}

void
StochasticPorousMedia::computeProperties()
{	
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
	//porous media
    _permeability[qp]         = _init_permeability[qp];
    _porosity[qp]             = _input_porosity;
    _density_rock[qp]         = _input_density_rock;
      
    //chem. reactions
    _diffusivity[qp]     = _input_chem_diff;
      
    //gravity    
    _gravity_vector[qp](0) = _gx; 
    _gravity_vector[qp](1) = _gy;
    _gravity_vector[qp](2) = _gz;
    _gravity[qp]           = _input_gravity;
      
      // if dissolution or precipitation is taking place, we need to adjust permeability and porosity accordingly
      if (_has_chem_reactions)
      {
          if (_vals.size())
          {
              Real _initial_vf = 1.0;
              Real _vf = 1.0;
              
              for (unsigned int i=0; i<_vals.size(); ++i)
              {
                  _initial_vf += 1.0e-3*_mineral[i]*_molecular_weight[i]/_mineral_density[i];
                  
                  _vf += 1.0e-3*(*_vals[i])[qp]*_molecular_weight[i]/_mineral_density[i];
                  
              }
              // Update porosity
              _porosity[qp] = _initial_vf *_input_porosity/_vf;
          }
          
          if (_porosity[qp] < 1.0e-3)
              _porosity[qp]=1.0e-3;
          
          // Permeability changes calculated from porosity changes according to Carman-Kozeny relationship k=ki*(1-ni)^2 * (n/ni)^3 / (1-n)^2
          _permeability[qp] = _init_permeability[qp] * (1.0-_input_porosity) * (1.0-_input_porosity) * std::pow(_porosity[qp]/_input_porosity,3)/(1.0-_porosity[qp])/(1.0-_porosity[qp]);
          
          // Permeability changes calculated from porosity changes according to Power Law with an order of 5.2: k=ki* (n/ni)^5.2
          //_permeability[qp] = inpit_permeability[qp] * std::pow(_porosity[qp]/_input_porosity, 5.2);
          
          // The diffusivity used in the kernels (already multiplied by porosity)
          _diffusivity[qp] = _input_chem_diff*_porosity[qp];
      }      
  }
}
