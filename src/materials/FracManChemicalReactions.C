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

#include "FracManChemicalReactions.h"

template<>
InputParameters validParams<FracManChemicalReactions>()
{
  InputParameters params = validParams<FracManPorousMedia>();
  ////Matrix
  params.addParam<Real>("matrix_diffusivity", 1e-8, "the chemical diffusivity of the matrix, [m^2/s]");
  params.addParam<std::vector<Real> >("matrix_mineral", std::vector<Real>(1, 16.65), "Initial mineral concentration in matrix, [mol/L] solution");
  params.addParam<std::vector<Real> >("matrix_molecular_weight", std::vector<Real>(1, 100.08), "The molecular weight of mineral in the matrix, [g/mol]");
  params.addParam<std::vector<Real> >("matrix_mineral_density", std::vector<Real>(1, 2.5), "The density of mineral in the matrix, [g/cm^3]");
    
  ////Fractures
  params.addRequiredParam<std::vector<int> >("fracture_numbers","The number associated with each of the fractures you would like to include from the FracMan file");
  params.addParam<std::vector<Real> >("fracture_diffusivity", std::vector<Real>(1, 1.0e-8), "The chemical diffusivity of the fractures, [m^2/s]");
  params.addParam<std::vector<Real> >("fracture_mineral", std::vector<Real>(1, 16.65), "Initial mineral concentration in fractures, [mol/L] solution");
  params.addParam<std::vector<Real> >("fracture_molecular_weight", std::vector<Real>(1, 100.08), "The molecular weight of mineral in fractures, [g/mol]");
  params.addParam<std::vector<Real> >("fracture_mineral_density", std::vector<Real>(1, 2.5), "The density of mineral in fractures, [g/cm^3]");
  params.addCoupledVar("v", "caco3");
  
  return params;
}

FracManChemicalReactions::FracManChemicalReactions(const std::string & name,
                         InputParameters parameters)
  :FracManPorousMedia(name, parameters),
////Grab user input parameters
   //Matrix
   _matrix_chem_diff(getParam<Real>("matrix_diffusivity")),
   _matrix_mineral(getParam<std::vector<Real> >("matrix_mineral")),
   _matrix_molecular_weight(getParam<std::vector<Real> >("matrix_molecular_weight")),
   _matrix_mineral_density(getParam<std::vector<Real> >("matrix_mineral_density")),
   //Fractures
   _fracture_number_vec(getParam<std::vector<int> >("fracture_numbers")),
   _fracture_chem_diff_vec(getParam<std::vector<Real> >("fracture_diffusivity")),
   _fracture_mineral_vec(getParam<std::vector<Real> >("fracture_mineral")),
   _fracture_molecular_weight_vec(getParam<std::vector<Real> >("fracture_molecular_weight")),
   _fracture_mineral_density_vec(getParam<std::vector<Real> >("fracture_mineral_density")),

////Delcare material properties
   _diffusivity(declareProperty<Real>("diffusivity"))

{
    num_frac_vec_entries        = _fracture_number_vec.size();
    num_chem_diff_vec_entries   = _fracture_chem_diff_vec.size();
    num_mineral_vec_entries     = _fracture_mineral_vec.size();
    num_mol_weight_vec_entries  = _fracture_molecular_weight_vec.size();
    num_min_dens_vec_entries    = _fracture_mineral_density_vec.size();

    // we want either one value of diffusivity to assign to all fractures or an individual value to assign to each fracture
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
FracManChemicalReactions::computeProperties()
{
    if (!areParentPropsComputed())
        FracManPorousMedia::computeProperties();
    
    for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
    {
        //material property assignment for matrix
        if (_fracture_map[qp] == 0)
        {
            _diffusivity[qp]          = _matrix_chem_diff;
            
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
            //Update porosity
            if (_porosity[qp] < 1.0e-3)
                _porosity[qp]=1.0e-3;
            
            // Permeability changes calculated from porosity changes according to Carman-Kozeny relationship k=ki*(1-ni)^2 * (n/ni)^3 / (1-n)^2
            _permeability[qp] = _matrix_permeability * (1.0-_matrix_porosity) * (1.0-_matrix_porosity) * std::pow(_porosity[qp]/_matrix_porosity,3)/(1.0-_porosity[qp])/(1.0-_porosity[qp]);
                
            // The diffusivity used in the kernels (already multiplied by porosity)
            _diffusivity[qp] = _matrix_chem_diff*_porosity[qp];
        }
        //material property assignment for each of the fractures
        for (unsigned int k = 0; k < num_frac_vec_entries; k++)
        {
            if (_fracture_map[qp] == _fracture_number_vec[k])
            {
                //If only one chem_diff value was provided, assign this value to all fractures
                //Else cycle through each fracture and assign values in order provided
                if (num_chem_diff_vec_entries < 2)
                    _diffusivity[qp]      = _fracture_chem_diff_vec[0];
                else
                    _diffusivity[qp]      = _fracture_chem_diff_vec[k];
                
                // if dissolution or precipitation is taking place, we need to adjust permeability and porosity accordingly
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
}
