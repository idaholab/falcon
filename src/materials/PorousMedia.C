#include "PorousMedia.h"

template<>
InputParameters validParams<PorousMedia>()
{
  InputParameters params = validParams<Material>();
  params.addParam<Real>("permeability",1.0e-12, "intrinsic permeability in m^2");
  params.addParam<Real>("porosity", 0.2, "rock porosity");
  params.addParam<Real>("rho_r", 2.50e3, "rock density in Kg/m^3");
  
  params.addParam<Real>("gravity",9.80665,"gravity acceleration constant");
  params.addParam<Real>("gx",0.0,"x component of the gravity pressure vector");
  params.addParam<Real>("gy",0.0,"y component of the gravity pressure vector");
  params.addParam<Real>("gz",1.0,"z component of the gravity pressure vector");
  
  return params;
}

PorousMedia::PorousMedia(const std::string & name,
                         MooseSystem & moose_system,
                         InputParameters parameters)
  :Material(name, moose_system, parameters),
     _input_permeability(getParam<Real>("permeability")),
     _input_porosity(getParam<Real>("porosity")),
     _input_rho_r(getParam<Real>("rho_r")),
     
     _input_gravity(getParam<Real>("gravity")),
     _gx(getParam<Real>("gx")),
     _gy(getParam<Real>("gy")),
     _gz(getParam<Real>("gz")),

   //delcare material properties
     _permeability(declareProperty<Real>("permeability")),
     _porosity(declareProperty<Real>("porosity")),
     _rho_r(declareProperty<Real>("rho_r")),

     _gravity(declareProperty<Real>("gravity")),
     _gravity_vector(declareProperty<RealVectorValue>("gravity_vector"))
{ }

void
PorousMedia::computeProperties()
{
  for(unsigned int qp=0; qp<_n_qpoints; qp++)
  {
//rock properties
    _permeability[qp]         = _input_permeability;
    _porosity[qp]             = _input_porosity;
    _rho_r[qp]                = _input_rho_r;    

//  gravity    
    _gravity_vector[qp](0) = _gx; 
    _gravity_vector[qp](1) = _gy;
    _gravity_vector[qp](2) = _gz;
    _gravity[qp]           = _input_gravity;
  }
}
