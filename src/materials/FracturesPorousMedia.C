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

#include "FracturesPorousMedia.h"
#include "Transient.h"

template<>
InputParameters validParams<FracturesPorousMedia>()
{
  InputParameters params = validParams<Material>();
////Matrix
  //rock property inputs
  params.addParam<Real>("matrix_permeability",1.0e-12, "intrinsic permeability of matrix (m^2)");
  params.addParam<Real>("matrix_porosity", 0.3, "rock porosity of matrix");
  params.addParam<Real>("matrix_density", 2.50e3, "rock density of matrix (kg/m^3)");
  //chem reaction inputs
  params.addParam<Real>("matrix_diffusivity", 1e-8, "the chemical diffusivity of the matrix, [m^2/s]");
  params.addParam<std::vector<Real> >("matrix_mineral", "Initial mineral concentration in matrix, [mol/L] solution");
  params.addParam<std::vector<Real> >("matrix_molecular_weight", "The molecular weight of mineral in the matrix, [g/mol]");
  params.addParam<std::vector<Real> >("matrix_mineral_density", "The density of mineral in the matrix, [g/cm^3]");

////Gravity
  params.addParam<Real>("gravity",9.80665,"gravity acceleration constant");
  params.addParam<Real>("gx",0.0,"x component of the gravity pressure vector");
  params.addParam<Real>("gy",0.0,"y component of the gravity pressure vector");
  params.addParam<Real>("gz",1.0,"z component of the gravity pressure vector");
    
////Fractures
  //fracture map/number inputs
  params.addRequiredCoupledVar("fractures", "coupled aux variable that maps where the fracture are");
  params.addParam<Real>("fracture_num", 0, "number in fracture map that indicates fractures");
  params.addParam<Real>("matrix_num", 1, "number in fracture map that indicates matrix");
  params.addParam<Real>("model_fracture_aperture", 1.0, "width of fracture/high permeability area in the model");
  //rock property inputs
  params.addParam<Real>("fracture_permeability", "intrinsic permeability of fractures (m^2)");
  params.addParam<Real>("fracture_porosity", "rock porosity of fractures");
  params.addParam<Real>("fracture_density", "rock density of fractures (kg/m^3)");
  //chem reaction coupled variables and parameters
  params.addParam<Real>("fracture_diffusivity", "the chemical diffusivity of the fractures, [m^2/s]");
  params.addParam<std::vector<Real> >("fracture_mineral", "Initial mineral concentration in fractures, [mol/L] solution");
  params.addParam<std::vector<Real> >("fracture_molecular_weight", "The molecular weight of mineral in fractures, [g/mol]");
  params.addParam<std::vector<Real> >("fracture_mineral_density", "The density of mineral in fractures, [g/cm^3]");
  params.addParam<bool>("has_chem_reactions", false, "flag if chemical reactions are present");
  params.addCoupledVar("v", "caco3");
    
  return params;
}

FracturesPorousMedia::FracturesPorousMedia(const std::string & name,
                         InputParameters parameters)
  :Material(name, parameters),
////Grab user input parameters
    //Matrix
    //rock property inputs
    _matrix_permeability(getParam<Real>("matrix_permeability")),
    _matrix_porosity(getParam<Real>("matrix_porosity")),
    _matrix_density(getParam<Real>("matrix_density")),
    //chem reaction inputs
    _matrix_chem_diff(getParam<Real>("matrix_diffusivity")),
    _matrix_mineral(getParam<std::vector<Real> >("matrix_mineral")),
    _matrix_molecular_weight(getParam<std::vector<Real> >("matrix_molecular_weight")),
    _matrix_mineral_density(getParam<std::vector<Real> >("matrix_mineral_density")),

    ////Gravity
    _input_gravity(getParam<Real>("gravity")),
    _gx(getParam<Real>("gx")),
    _gy(getParam<Real>("gy")),
    _gz(getParam<Real>("gz")),

    ////Fractures
    //fracture map inputs
    _has_fractures(isCoupled("fractures")),
    _fractures(_has_fractures ? coupledValue("fractures") : _zero),
    _fracture_num(getParam<Real>("fracture_num")),
    _matrix_num(getParam<Real>("matrix_num")),
    _model_fracture_aperture(getParam<Real>("model_fracture_aperture")),
    //rock property inputs
    _fracture_permeability(getParam<Real>("fracture_permeability")),
    _fracture_porosity(getParam<Real>("fracture_porosity")),
    _fracture_density(getParam<Real>("fracture_density")),
    //chem reaction inputs
    _fracture_chem_diff(getParam<Real>("fracture_diffusivity")),
    _fracture_mineral(getParam<std::vector<Real> >("fracture_mineral")),
    _fracture_molecular_weight(getParam<std::vector<Real> >("fracture_molecular_weight")),
    _fracture_mineral_density(getParam<std::vector<Real> >("fracture_mineral_density")),
    _has_chem_reactions(getParam<bool>("has_chem_reactions")),

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
FracturesPorousMedia::computeProperties()
{
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
      //material property assignment for matrix
      if (_fractures[qp] == _matrix_num)
      {
          _permeability[qp]         = _matrix_permeability;
          _porosity[qp]             = _matrix_porosity;
          _density_rock[qp]         = _matrix_density;
          _diffusivity[qp]          = _matrix_chem_diff;
          
          // if dissolution or precipitation is taking place, we need to adjust permeability and porosity accordingly
          if (_has_chem_reactions)
          {
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
      //material property assignment for fractures
      else if (_fractures[qp] == _fracture_num)
      {
          Real aperture = sqrt(12 * _fracture_permeability);
          
          _permeability[qp]         = std::pow(aperture , 3) / (12 * _model_fracture_aperture);
          _porosity[qp]             = _fracture_porosity;
          _density_rock[qp]         = _fracture_density;
          _diffusivity[qp]          = _fracture_chem_diff;
          
          // if dissolution or precipitation is taking place, we need to adjust permeability and porosity accordingly
          if (_has_chem_reactions)
          {
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
              _permeability[qp] = std::pow(aperture , 3) / (12 * _model_fracture_aperture);

              // The diffusivity used in the kernels (already multiplied by porosity)
              _diffusivity[qp] = _matrix_chem_diff*_porosity[qp];
          }
      }
      //material property assignment for anything else will get lumped in with the matrix
      else
      {
          _permeability[qp]         = _matrix_permeability;
          _porosity[qp]             = _matrix_porosity;
          _density_rock[qp]         = _matrix_density;
          _diffusivity[qp]          = _matrix_chem_diff;
          
          // if dissolution or precipitation is taking place, we need to adjust permeability and porosity accordingly
          if (_has_chem_reactions)
          {
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
  
      _gravity_vector[qp](0) = _gx;
      _gravity_vector[qp](1) = _gy;
      _gravity_vector[qp](2) = _gz;
      _gravity[qp]           = _input_gravity;
  }
}
