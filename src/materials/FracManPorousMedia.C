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
  params.addParam<Real>("density_rock", 2.50e3, "rock density in Kg/m^3");
  params.addParam<Real>("matrix_permeability",1.0e-12, "intrinsic permeability in m^2");
  params.addParam<Real>("matrix_porosity", 0.3, "Rock porosity set from the PorousMedia material kernel");
     
  params.addParam<Real>("gravity",9.80665,"gravity acceleration constant");
  params.addParam<Real>("gx",0.0,"x component of the gravity pressure vector");
  params.addParam<Real>("gy",0.0,"y component of the gravity pressure vector");
  params.addParam<Real>("gz",1.0,"z component of the gravity pressure vector");

  params.addCoupledVar("x_disp", "TODO: ad description");
  params.addCoupledVar("y_disp", "TODO: ad description");
  params.addCoupledVar("z_disp", "TODO: ad description");

  params.addRequiredParam<std::vector<int> >("fracture_numbers","The number associated with each of the fractures you would like to include from the FracMan file");
  params.addRequiredParam<std::vector<Real> >("fracture_permeabilities","The permeability values associated with each of the fractures");
  params.addRequiredParam<std::vector<Real> >("fracture_porosities","The porosity values associated with each of the fractures");
  params.addCoupledVar("fracture_map", "bleh");
  
  return params;
}

FracManPorousMedia::FracManPorousMedia(const std::string & name,
                         InputParameters parameters)
  :Material(name, parameters),
_input_density_rock(getParam<Real>("density_rock")),
_matrix_permeability(getParam<Real>("matrix_permeability")),
_matrix_porosity(getParam<Real>("matrix_porosity")),

   _input_gravity(getParam<Real>("gravity")),
   _gx(getParam<Real>("gx")),
   _gy(getParam<Real>("gy")),
   _gz(getParam<Real>("gz")),

//delcare material properties
   _permeability(declareProperty<Real>("permeability")),
   _material_porosity(declareProperty<Real>("material_porosity")),
   _density_rock(declareProperty<Real>("density_rock")),
   _gravity(declareProperty<Real>("gravity")),
   _gravity_vector(declareProperty<RealVectorValue>("gravity_vector")),

_fracture_number_vec(getParam<std::vector<int> >("fracture_numbers")),
_fracture_permeability_vec(getParam<std::vector<Real> >("fracture_permeabilities")),
_fracture_porosity_vec(getParam<std::vector<Real> >("fracture_porosities")),
_fracture_map(coupledValue("fracture_map")),

   _already_computed(false)

{
    num_frac_vec_entries = _fracture_number_vec.size();
    num_perm_vec_entries = _fracture_permeability_vec.size();
    num_poro_vec_entries = _fracture_porosity_vec.size();
}

void
FracManPorousMedia::computeProperties()
{

  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
      if (_fracture_map[qp] == 0)
      {
          _permeability[qp]         = _matrix_permeability;
          _material_porosity[qp]    = _matrix_porosity;
      }
      
      for (unsigned int k = 0; k < num_frac_vec_entries; k++)
      {
          if (_fracture_map[qp] == _fracture_number_vec[k])
          {
              if (num_perm_vec_entries < 2)
                  _permeability[qp] = _fracture_permeability_vec[0];
              else
                  _permeability[qp]         = _fracture_permeability_vec[k];
              
              if (num_poro_vec_entries <2)
                  _material_porosity[qp] = _fracture_porosity_vec[0];
              else
                  _material_porosity[qp]    = _fracture_porosity_vec[k];
          }
      }

      _density_rock[qp]         = _input_density_rock;

//  gravity    
    _gravity_vector[qp](0) = _gx; 
    _gravity_vector[qp](1) = _gy;
    _gravity_vector[qp](2) = _gz;
    _gravity[qp]           = _input_gravity;
  }
}
