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

#include "FracturesChemicalReactions.h"
#include "Transient.h"

template<>
InputParameters validParams<FracturesChemicalReactions>()
{
  InputParameters params = validParams<FracturesPorousMedia>();
////Matrix
  params.addParam<Real>("matrix_diffusivity", 1e-8, "the chemical diffusivity of the matrix, [m^2/s]");
  params.addParam<std::vector<Real> >("matrix_mineral", std::vector<Real>(1, 16.65), "Initial mineral concentration in matrix, [mol/L] solution");
  params.addParam<std::vector<Real> >("matrix_molecular_weight", std::vector<Real>(1, 100.08), "The molecular weight of mineral in the matrix, [g/mol]");
  params.addParam<std::vector<Real> >("matrix_mineral_density", std::vector<Real>(1, 2.5), "The density of mineral in the matrix, [g/cm^3]");
    
////Fractures
  //chem reaction coupled variables and parameters
  params.addParam<Real>("fracture_diffusivity", 1.0e-8, "the chemical diffusivity of the fractures, [m^2/s]");
  params.addParam<std::vector<Real> >("fracture_mineral", std::vector<Real>(1, 16.65), "Initial mineral concentration in fractures, [mol/L] solution");
  params.addParam<std::vector<Real> >("fracture_molecular_weight", std::vector<Real>(1, 100.08), "The molecular weight of mineral in fractures, [g/mol]");
  params.addParam<std::vector<Real> >("fracture_mineral_density", std::vector<Real>(1, 2.5), "The density of mineral in fractures, [g/cm^3]");
  params.addCoupledVar("v", "caco3");
    
  return params;
}

FracturesChemicalReactions::FracturesChemicalReactions(const std::string & name,
                         InputParameters parameters)
  :FracturesPorousMedia(name, parameters),
////Grab user input parameters
    //Matrix
    //rock property inputs
    _matrix_permeability(getParam<Real>("matrix_permeability")),
    _matrix_porosity(getParam<Real>("matrix_porosity")),
    //chem reaction inputs
    _matrix_chem_diff(getParam<Real>("matrix_diffusivity")),
    _matrix_mineral(getParam<std::vector<Real> >("matrix_mineral")),
    _matrix_molecular_weight(getParam<std::vector<Real> >("matrix_molecular_weight")),
    _matrix_mineral_density(getParam<std::vector<Real> >("matrix_mineral_density")),

    ////Fractures
    //fracture map inputs
    _fracture_num(getParam<Real>("fracture_num")),
    _model_fracture_aperture(getParam<Real>("model_fracture_aperture")),
    //rock property inputs
    _fracture_permeability(getParam<Real>("fracture_permeability")),
    _fracture_porosity(getParam<Real>("fracture_porosity")),
    //chem reaction inputs
    _fracture_chem_diff(getParam<Real>("fracture_diffusivity")),
    _fracture_mineral(getParam<std::vector<Real> >("fracture_mineral")),
    _fracture_molecular_weight(getParam<std::vector<Real> >("fracture_molecular_weight")),
    _fracture_mineral_density(getParam<std::vector<Real> >("fracture_mineral_density")),

////Delcare material properties
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
FracturesChemicalReactions::computeProperties()
{
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
      //material property assignment for fractures
      if (_fractures[qp] == _fracture_num)
      {          
          // if dissolution or precipitation is taking place, we need to adjust permeability and porosity accordingly
          if (_vals.size())
          {
              Real _initial_vf = 1.0;
              Real _vf = 1.0;
              
              for (unsigned int i=0; i<_vals.size(); ++i)
              {
                  _initial_vf += 1.0e-3*_fracture_mineral[i]*_fracture_molecular_weight[i]/_fracture_mineral_density[i];
                  _vf += 1.0e-3*(*_vals[i])[qp]*_fracture_molecular_weight[i]/_fracture_mineral_density[i];
              }
              _porosity[qp] = _initial_vf *_fracture_porosity/_vf;
          }
          // Update porosity
          if (_porosity[qp] < 1.0e-3)
              _porosity[qp]=1.0e-3;
          
          // Permeability changes calculated from porosity changes according to Carman-Kozeny relationship k=ki*(1-ni)^2 * (n/ni)^3 / (1-n)^2
          Real aperture = sqrt(12 * (_fracture_permeability * (1.0-_fracture_porosity) * (1.0-_fracture_porosity) * std::pow(_porosity[qp]/_fracture_porosity,3)/(1.0-_porosity[qp])/(1.0-_porosity[qp])));
          Real fracture_ratio = _model_fracture_aperture / aperture;
          _permeability[qp] = ((std::pow(_model_fracture_aperture,2)) * (std::pow((1/fracture_ratio) , 3)))/12;

          // The diffusivity used in the kernels (already multiplied by porosity)
          _diffusivity[qp] = _fracture_chem_diff * _porosity[qp];
      }
      //material property assignment for matrix
      else
      {          
          // if dissolution or precipitation is taking place, we need to adjust permeability and porosity accordingly
          if (_vals.size())
          {
              Real _initial_vf = 1.0;
              Real _vf = 1.0;
                  
              for (unsigned int i=0; i<_vals.size(); ++i)
              {
                  _initial_vf += 1.0e-3*_matrix_mineral[i]*_matrix_molecular_weight[i]/_matrix_mineral_density[i];
                  _vf += 1.0e-3*(*_vals[i])[qp]*_matrix_molecular_weight[i]/_matrix_mineral_density[i];
              }
              _porosity[qp] = _initial_vf *_matrix_porosity/_vf;
          }
          // Update porosity
          if (_porosity[qp] < 1.0e-3)
              _porosity[qp]=1.0e-3;
          
          // Permeability changes calculated from porosity changes according to Carman-Kozeny relationship k=ki*(1-ni)^2 * (n/ni)^3 / (1-n)^2
          _permeability[qp] = _matrix_permeability * (1.0-_matrix_porosity) * (1.0-_matrix_porosity) * std::pow(_porosity[qp]/_matrix_porosity,3)/(1.0-_porosity[qp])/(1.0-_porosity[qp]);
          
          // The diffusivity used in the kernels (already multiplied by porosity)
          _diffusivity[qp] = _matrix_chem_diff*_porosity[qp];
      }
  }
}
