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
  params.addParam<Real>("matrix_permeability",1.0e-12, "intrinsic permeability of matrix, [m^2]");
  params.addParam<Real>("matrix_porosity", 0.3, "rock porosity of matrix");
  params.addParam<Real>("matrix_density", 2.50e3, "rock density of matrix (kg/m^3)");
    
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
    
  return params;
}

FracManPorousMedia::FracManPorousMedia(const std::string & name,
                         InputParameters parameters)
  :Material(name, parameters),
////Grab user input parameters
    ////Matrix
    _matrix_permeability(getParam<Real>("matrix_permeability")),
    _matrix_porosity(getParam<Real>("matrix_porosity")),
    _matrix_density(getParam<Real>("matrix_density")),

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

////Delcare material properties
    //rock material props
    _permeability(declareProperty<Real>("permeability")),
    _porosity(declareProperty<Real>("porosity")),
    _density_rock(declareProperty<Real>("density_rock")),
    //gravity material props
    _gravity(declareProperty<Real>("gravity")),
    _gravity_vector(declareProperty<RealVectorValue>("gravity_vector")),

    _already_computed(false)

{
    // storing the number of vetor entries into respective local variables
    num_frac_vec_entries        = _fracture_number_vec.size();
    num_perm_vec_entries        = _fracture_permeability_vec.size();
    num_poro_vec_entries        = _fracture_porosity_vec.size();
    num_dens_vec_entries        = _fracture_density_vec.size();
        
    // we want either one value of perm/poro/density/diffusivity to assign to all fractures or an individual value to assign to each fracture
    if (((num_perm_vec_entries > 2) && (num_perm_vec_entries < num_frac_vec_entries)) || (num_perm_vec_entries > num_frac_vec_entries))
        mooseError("You must provide either one permeability value for all fractures or a permeability value for each fracture");
    if (((num_poro_vec_entries > 2) && (num_poro_vec_entries < num_frac_vec_entries)) || (num_poro_vec_entries > num_frac_vec_entries))
        mooseError("You must provide either one porosity value for all fractures or a porosity value for each fracture");
    if (((num_dens_vec_entries > 2) && (num_dens_vec_entries < num_frac_vec_entries)) || (num_dens_vec_entries > num_frac_vec_entries))
        mooseError("You must provide either one rock_density value for all fractures or a rock_density value for each fracture");
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
            }
      }

//  gravity    
    _gravity_vector[qp](0) = _gx; 
    _gravity_vector[qp](1) = _gy;
    _gravity_vector[qp](2) = _gz;
    _gravity[qp]           = _input_gravity;
  }
}
