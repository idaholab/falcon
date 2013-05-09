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

#include "PorousMedia.h"
#include "Transient.h"

template<>
InputParameters validParams<PorousMedia>()
{
  InputParameters params = validParams<Material>();
  params.addParam<Real>("permeability",1.0e-12, "intrinsic permeability in m^2");
  params.addParam<Real>("material_porosity", 0.3, "Rock porosity set from the PorousMedia material kernel");
  params.addParam<Real>("density_rock", 2.50e3, "rock density in Kg/m^3");
     
  params.addParam<Real>("gravity",9.80665,"gravity acceleration constant");
  params.addParam<Real>("gx",0.0,"x component of the gravity pressure vector");
  params.addParam<Real>("gy",0.0,"y component of the gravity pressure vector");
  params.addParam<Real>("gz",1.0,"z component of the gravity pressure vector");
  
  return params;
}

PorousMedia::PorousMedia(const std::string & name,
                         InputParameters parameters)
  :Material(name, parameters),
   _input_permeability(getParam<Real>("permeability")),
   _input_material_porosity(getParam<Real>("material_porosity")),
   _input_density_rock(getParam<Real>("density_rock")),

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
   _already_computed(false)

{ }

void
PorousMedia::computeProperties()
{

  Real aperture_old, aperture_change, aperture_new, porosity;
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {

    _permeability[qp]         = _input_permeability;
    _material_porosity[qp]    = _input_material_porosity;
    _density_rock[qp]         = _input_density_rock;    
      
//  gravity    
    _gravity_vector[qp](0) = _gx; 
    _gravity_vector[qp](1) = _gy;
    _gravity_vector[qp](2) = _gz;
    _gravity[qp]           = _input_gravity;
  }
}
