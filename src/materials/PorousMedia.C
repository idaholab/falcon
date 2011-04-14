#include "PorousMedia.h"

template<>
InputParameters validParams<PorousMedia>()
{
  InputParameters params = validParams<Material>();
  params.addParam<Real>("permeability",1.0e-12, "intrinsic permeability in m^2");
//  params.addParam<Real>("porosity", 0.2, "rock porosity");
  params.addParam<Real>("density_rock", 2.50e3, "rock density in Kg/m^3");
  
  params.addParam<Real>("gravity",9.80665,"gravity acceleration constant");
  params.addParam<Real>("gx",0.0,"x component of the gravity pressure vector");
  params.addParam<Real>("gy",0.0,"y component of the gravity pressure vector");
  params.addParam<Real>("gz",1.0,"z component of the gravity pressure vector");

  params.addCoupledVar("porosity", "Coupled NodalAux used for porosity");
  
  return params;
}

PorousMedia::PorousMedia(const std::string & name,
                         InputParameters parameters)
  :Material(name, parameters),
     _input_permeability(getParam<Real>("permeability")),
   //    _input_porosity(getParam<Real>("porosity")),
     _input_density_rock(getParam<Real>("density_rock")),
     
     _input_gravity(getParam<Real>("gravity")),
     _gx(getParam<Real>("gx")),
     _gy(getParam<Real>("gy")),
     _gz(getParam<Real>("gz")),

      _porosity(coupledValue("porosity")),

   //delcare material properties
     _permeability(declareProperty<Real>("permeability")),
   //  _porosity(declareProperty<Real>("porosity")),
     _density_rock(declareProperty<Real>("density_rock")),

     _gravity(declareProperty<Real>("gravity")),
     _gravity_vector(declareProperty<RealVectorValue>("gravity_vector"))
{ }

void
PorousMedia::computeProperties()
{
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
//rock properties
    _permeability[qp]         = _input_permeability;
    //  _porosity[qp]             = _input_porosity;
    _density_rock[qp]         = _input_density_rock;    

//  gravity    
    _gravity_vector[qp](0) = _gx; 
    _gravity_vector[qp](1) = _gy;
    _gravity_vector[qp](2) = _gz;
    _gravity[qp]           = _input_gravity;
  }
}
