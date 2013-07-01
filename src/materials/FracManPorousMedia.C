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

#include "FracManPorousMedia.h"
#include "Transient.h"

template<>
InputParameters validParams<FracManPorousMedia>()
{
  InputParameters params = validParams<Material>();
////Matrix
  //rock property inputs
  params.addParam<Real>("matrix_permeability",1.0e-12, "intrinsic permeability of matrix, [m^2]");
  params.addParam<Real>("matrix_porosity", 0.3, "rock porosity of matrix");
  params.addParam<Real>("matrix_density", 2.50e3, "rock density of matrix (kg/m^3)");
  //chem reaction inputs
  params.addParam<Real>("matrix_diffusivity", 1e-8, "the chemical diffusivity of the matrix, [m^2/s]");
  params.addParam<std::vector<Real> >("matrix_mineral", std::vector<Real>(1, 16.65), "Initial mineral concentration in matrix, [mol/L] solution");
  params.addParam<std::vector<Real> >("matrix_molecular_weight", std::vector<Real>(1, 100.08), "The molecular weight of mineral in the matrix, [g/mol]");
  params.addParam<std::vector<Real> >("matrix_mineral_density", std::vector<Real>(1, 2.5), "The density of mineral in the matrix, [g/cm^3]");
    
////Gravity
  params.addParam<Real>("gravity",9.80665,"gravity acceleration constant, [m/s^2]");
  params.addParam<Real>("gx",0.0,"x component of the gravity pressure vector");
  params.addParam<Real>("gy",0.0,"y component of the gravity pressure vector");
  params.addParam<Real>("gz",1.0,"z component of the gravity pressure vector");

////Fractures
  //fracture map/number inputs
  params.addCoupledVar("fracture_map", "AuxVariable that serves as a map of which elements are fractures or matrix from the FracMan input file, unique numbers are assigned to each fracture");
  params.addRequiredParam<std::vector<int> >("fracture_numbers","The number associated with each of the fractures you would like to include from the FracMan file");
  //rock property inputs
  params.addParam<std::vector<Real> >("fracture_permeabilities", std::vector<Real>(1, 1.0e-12), "The permeability values associated with each of the fractures, [m^2]");
  params.addParam<std::vector<Real> >("fracture_porosities", std::vector<Real>(1, 0.3), "The porosity values associated with each of the fractures");
  params.addParam<std::vector<Real> >("fracture_densities", std::vector<Real>(1, 2500), "The rock density values associated with each of the fractures, [kg/m^3]");
  //chem reaction coupled variables and parameters
  params.addParam<std::vector<Real> >("fracture_diffusivity", std::vector<Real>(1, 1.0e-8), "The chemical diffusivity of the fractures, [m^2/s]");
  params.addParam<std::vector<Real> >("fracture_mineral", std::vector<Real>(1, 16.65), "Initial mineral concentration in fractures, [mol/L] solution");
  params.addParam<std::vector<Real> >("fracture_molecular_weight", std::vector<Real>(1, 100.08), "The molecular weight of mineral in fractures, [g/mol]");
  params.addParam<std::vector<Real> >("fracture_mineral_density", std::vector<Real>(1, 2.5), "The density of mineral in fractures, [g/cm^3]");
  params.addParam<bool>("has_chem_reactions", false, "flag true if chemical reactions are present");
  params.addCoupledVar("v", "caco3");
    
  return params;
}

FracManPorousMedia::FracManPorousMedia(const std::string & name,
                         InputParameters parameters)
  :Material(name, parameters),
////Grab user input parameters
    ////Matrix
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
    _fracture_map(coupledValue("fracture_map")),
    _fracture_number_vec(getParam<std::vector<int> >("fracture_numbers")),
    //rock property inputs
    _fracture_permeability_vec(getParam<std::vector<Real> >("fracture_permeabilities")),
    _fracture_porosity_vec(getParam<std::vector<Real> >("fracture_porosities")),
    _fracture_density_vec(getParam<std::vector<Real> >("fracture_densities")),
    //chem reaction inputs
    _fracture_chem_diff_vec(getParam<std::vector<Real> >("fracture_diffusivity")),
    _fracture_mineral_vec(getParam<std::vector<Real> >("fracture_mineral")),
    _fracture_molecular_weight_vec(getParam<std::vector<Real> >("fracture_molecular_weight")),
    _fracture_mineral_density_vec(getParam<std::vector<Real> >("fracture_mineral_density")),
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
    // storing the number of vetor entries into respective local variables
    num_frac_vec_entries        = _fracture_number_vec.size();
    num_perm_vec_entries        = _fracture_permeability_vec.size();
    num_poro_vec_entries        = _fracture_porosity_vec.size();
    num_dens_vec_entries        = _fracture_density_vec.size();
    num_chem_diff_vec_entries   = _fracture_chem_diff_vec.size();
    num_mineral_vec_entries     = _fracture_mineral_vec.size();
    num_mol_weight_vec_entries  = _fracture_molecular_weight_vec.size();
    num_min_dens_vec_entries    = _fracture_mineral_density_vec.size();
    
    // we want either one value of perm/poro/density/diffusivity to assign to all fractures or an individual value to assign to each fracture
    if (((num_perm_vec_entries > 2) && (num_perm_vec_entries < num_frac_vec_entries)) || (num_perm_vec_entries > num_frac_vec_entries))
        mooseError("You must provide either one permeability value for all fractures or a permeability value for each fracture");
    if (((num_poro_vec_entries > 2) && (num_poro_vec_entries < num_frac_vec_entries)) || (num_poro_vec_entries > num_frac_vec_entries))
        mooseError("You must provide either one porosity value for all fractures or a porosity value for each fracture");
    if (((num_dens_vec_entries > 2) && (num_dens_vec_entries < num_frac_vec_entries)) || (num_dens_vec_entries > num_frac_vec_entries))
        mooseError("You must provide either one rock_density value for all fractures or a rock_density value for each fracture");
    if (((num_chem_diff_vec_entries > 2) && (num_chem_diff_vec_entries < num_frac_vec_entries)) || (num_chem_diff_vec_entries > num_frac_vec_entries))
        mooseError("You must provide either one diffusivity value for all fractures or a diffusivity value for each fracture");

    //resize and fill in _vals with number of provided chem species
    int n = coupledComponents("v");
    _vals.resize(n);
    for (unsigned int i=0; i<_vals.size(); ++i)
        _vals[i] = &coupledValue("v", i);
    
    //number of species provided
    num_vals = _vals.size();
    
    // we want either one value per species of mineral/molecular_weight/mineral_density to assign to all fractures or an individual value per species to assign to each fracture
    if (((num_mineral_vec_entries > 2*num_vals) && (num_mineral_vec_entries < num_frac_vec_entries*num_vals)) || (num_mineral_vec_entries > num_frac_vec_entries*num_vals))
        mooseError("You must provide either one mineral value per species for all fractures or a mineral value per species for each fracture");
    if (((num_mol_weight_vec_entries > 2*num_vals) && (num_mol_weight_vec_entries < num_frac_vec_entries*num_vals)) || (num_mol_weight_vec_entries > num_frac_vec_entries*num_vals))
        mooseError("You must provide either one molecular weight value per species for all fractures or a molecular weight value per species for each fracture");
    if (((num_min_dens_vec_entries > 2*num_vals) && (num_min_dens_vec_entries < num_frac_vec_entries*num_vals)) || (num_min_dens_vec_entries > num_frac_vec_entries*num_vals))
        mooseError("You must provide either one mineral density value per species for all fractures or a mineral density value per species for each fracture");
    
    // to make life easier, we ask that the number of mineral/molecular_weight/mineral_density values provided are the same
    if ((num_mineral_vec_entries != num_mol_weight_vec_entries) || (num_mineral_vec_entries != num_min_dens_vec_entries) || (num_mol_weight_vec_entries != num_min_dens_vec_entries))
        mooseError("Please provide the same number of fracture mineral/molecular_weight/mineral_density values, thanks!");
                                                      
}

void
FracManPorousMedia::computeProperties()
{

  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
      //material property assignment for matrix
      if (_fracture_map[qp] == 0)
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
              //Update porosity
              if (_porosity[qp] < 1.0e-3)
                  _porosity[qp]=1.0e-3;
              
              // Permeability changes calculated from porosity changes according to Carman-Kozeny relationship k=ki*(1-ni)^2 * (n/ni)^3 / (1-n)^2
              _permeability[qp] = _matrix_permeability * (1.0-_matrix_porosity) * (1.0-_matrix_porosity) * std::pow(_porosity[qp]/_matrix_porosity,3)/(1.0-_porosity[qp])/(1.0-_porosity[qp]);
              
              // The diffusivity used in the kernels (already multiplied by porosity)
              _diffusivity[qp] = _matrix_chem_diff*_porosity[qp];
          }
      }
      //material property assignment for each of the fractures
      for (unsigned int k = 0; k < num_frac_vec_entries; k++)
      {
          if (_fracture_map[qp] == _fracture_number_vec[k])
          {
              //If only one perm/poro/density/chem_diff value was provided, assign this value to all fractures
              //Else cycle through each fracture and assign values in order provided
              if (num_perm_vec_entries < 2)
                  _permeability[qp]     = _fracture_permeability_vec[0];
              else
                  _permeability[qp]     = _fracture_permeability_vec[k];
              
              if (num_poro_vec_entries < 2)
                  _porosity[qp]         = _fracture_porosity_vec[0];
              else
                  _porosity[qp]         = _fracture_porosity_vec[k];
              
              if (num_dens_vec_entries < 2)
                  _density_rock[qp]     = _fracture_density_vec[0];
              else
                  _density_rock[qp]     = _fracture_density_vec[k];
              
              if (num_chem_diff_vec_entries < 2)
                  _diffusivity[qp]      = _fracture_chem_diff_vec[0];
              else
                  _diffusivity[qp]      = _fracture_chem_diff_vec[k];
              
              // if dissolution or precipitation is taking place, we need to adjust permeability and porosity accordingly
              if (_has_chem_reactions)
              {
                  if (_vals.size())
                  {
                      Real _initial_vf = 1.0;
                      Real _vf = 1.0;
                      
                      // do this loop for each of the species provided
                      for (unsigned int i=0; i<_vals.size(); ++i)
                      {
                          // according to mooseError statements in constructor, at this point we should have an equal amount mineral/molecular_weight/mineral_density values
                          // therefor we can base our if statements based upon the size of just one of those vectors
                          // if there is only one value per species provided, then use just that one value for all fractures
                          // else if there is a value provided for each fracture for each species, then cycle through and use each of these for their respective fracture
                          if (num_mineral_vec_entries < 2)
                          {
                              _initial_vf += 1.0e-3*_fracture_mineral_vec[i]*_fracture_molecular_weight_vec[i]/_fracture_mineral_density_vec[i];
                              _vf += 1.0e-3*(*_vals[i])[qp]*_fracture_molecular_weight_vec[i]/_fracture_mineral_density_vec[i];
                          }
                          else
                          {
                              _initial_vf += 1.0e-3*_fracture_mineral_vec[i*k]*_fracture_molecular_weight_vec[i*k]/_fracture_mineral_density_vec[i*k];
                              _vf += 1.0e-3*(*_vals[i*k])[qp]*_fracture_molecular_weight_vec[i*k]/_fracture_mineral_density_vec[i*k];
                          }
                      }
                      // Update porosity
                      // if only one value for porosity was provided, use this values for all fractures
                      // else use values in order provided
                      if (num_poro_vec_entries < 2)
                          _porosity[qp] = _initial_vf *_fracture_porosity_vec[0]/_vf;
                      else
                          _porosity[qp] = _initial_vf *_fracture_porosity_vec[k]/_vf;
                  }
                
                  if (_porosity[qp] < 1.0e-3)
                      _porosity[qp] = 1.0e-3;
                  
                  // Permeability changes calculated from porosity changes according to Carman-Kozeny relationship k=ki*(1-ni)^2 * (n/ni)^3 / (1-n)^2
                  // if only one value for perm/poro was provided, use these values for all fractures
                  // else if multiple values for perm/poro were provided, use these values in order for each of the fractures
                  // else if a combo of the previous two was provided, 
                  if ((num_perm_vec_entries < 2) && (num_poro_vec_entries < 2))
                      _permeability[qp] = _fracture_permeability_vec[0] * (1.0-_fracture_porosity_vec[0]) * (1.0-_fracture_porosity_vec[0]) * std::pow(_porosity[qp]/_fracture_porosity_vec[0],3)/(1.0-_porosity[qp])/(1.0-_porosity[qp]);
                  else if ((num_perm_vec_entries > 2) && (num_poro_vec_entries > 2))
                      _permeability[qp] = _fracture_permeability_vec[k] * (1.0-_fracture_porosity_vec[k]) * (1.0-_fracture_porosity_vec[k]) * std::pow(_porosity[qp]/_fracture_porosity_vec[k],3)/(1.0-_porosity[qp])/(1.0-_porosity[qp]);
                  else if ((num_perm_vec_entries < 2) && (num_poro_vec_entries > 2))
                      _permeability[qp] = _fracture_permeability_vec[0] * (1.0-_fracture_porosity_vec[k]) * (1.0-_fracture_porosity_vec[k]) * std::pow(_porosity[qp]/_fracture_porosity_vec[k],3)/(1.0-_porosity[qp])/(1.0-_porosity[qp]);
                  else
                      _permeability[qp] = _fracture_permeability_vec[k] * (1.0-_fracture_porosity_vec[0]) * (1.0-_fracture_porosity_vec[0]) * std::pow(_porosity[qp]/_fracture_porosity_vec[0],3)/(1.0-_porosity[qp])/(1.0-_porosity[qp]);
                  
                  // The diffusivity used in the kernels (already multiplied by porosity)
                  if (num_chem_diff_vec_entries < 2)
                      _diffusivity[qp] = _fracture_chem_diff_vec[0] * _porosity[qp];
                  else
                      _diffusivity[qp] = _fracture_chem_diff_vec[k] * _porosity[qp];
              }
          }
      }

//  gravity    
    _gravity_vector[qp](0) = _gx; 
    _gravity_vector[qp](1) = _gy;
    _gravity_vector[qp](2) = _gz;
    _gravity[qp]           = _input_gravity;
  }
}
