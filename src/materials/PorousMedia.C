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

PorousMedia::PorousMedia(std::string name,
                         MooseSystem & moose_system,
                         InputParameters parameters)
  :Material(name, moose_system, parameters),
     _input_permeability(parameters.get<Real>("permeability")),
     _input_porosity(parameters.get<Real>("porosity")),
     _input_rho_r(parameters.get<Real>("rho_r")),
     
     _input_gravity(parameters.get<Real>("gravity")),
     _gx(parameters.get<Real>("gx")),
     _gy(parameters.get<Real>("gy")),
     _gz(parameters.get<Real>("gz")),

   //delcare material properties
     _permeability(declareRealProperty("permeability")),
     _porosity(declareRealProperty("porosity")),
     _rho_r(declareRealProperty("rho_r")),

     _gravity(declareRealProperty("gravity")),
     _gravity_vector(declareRealVectorValueProperty("gravity_vector"))
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
