#include "PorousMedia.h"
#include "Transient.h"
#include "SolidMechanics.h"

template<>
InputParameters validParams<PorousMedia>()
{
  InputParameters params = validParams<Material>();
  params.addParam<Real>("permeability",1.0e-12, "intrinsic permeability in m^2");
  params.addParam<Real>("material_porosity", 0.3, "Rock porosity set from the PorousMedia material kernel");
  params.addParam<Real>("density_rock", 2.50e3, "rock density in Kg/m^3");
 
  params.addParam<Real>("compressibility", 1e-5, "fluid compressibility Kg/m^3/Pa");
    
  params.addParam<Real>("gravity",9.80665,"gravity acceleration constant");
  params.addParam<Real>("gx",0.0,"x component of the gravity pressure vector");
  params.addParam<Real>("gy",0.0,"y component of the gravity pressure vector");
  params.addParam<Real>("gz",1.0,"z component of the gravity pressure vector");
	//rkp edits
  params.addParam<bool>("has_frac_perm",false,"switch for turning on/off disp dependent perm");
  params.addCoupledVar("z_disp", "modify permeability by z displacement");
  params.addParam<Real>("aperture",1.0e-7, "effective frac aperture");
//  params.addParam<RealVectorValue>("strain",1.0e-7, "effective frac aperture");	  
  
  return params;
}

PorousMedia::PorousMedia(const std::string & name,
                         InputParameters parameters)
  :Material(name, parameters),
   _input_permeability(getParam<Real>("permeability")),
   _input_material_porosity(getParam<Real>("material_porosity")),
   _input_density_rock(getParam<Real>("density_rock")),
   _input_compressibility(getParam<Real>("compressibility")),

   _input_gravity(getParam<Real>("gravity")),
   _gx(getParam<Real>("gx")),
   _gy(getParam<Real>("gy")),
   _gz(getParam<Real>("gz")),

//delcare material properties
   _permeability(declareProperty<Real>("permeability")),
   _material_porosity(declareProperty<Real>("material_porosity")),
   _density_rock(declareProperty<Real>("density_rock")),
   _compressibility(declareProperty<Real>("compressibility")),
   _gravity(declareProperty<Real>("gravity")),
   _gravity_vector(declareProperty<RealVectorValue>("gravity_vector")),

//rkp
   _has_frac_perm(getParam<bool>("has_frac_perm")),
   _z_disp( _has_frac_perm? coupledValue("z_disp"): _zero),
   _z_disp_old( _has_frac_perm? coupledValueOld("z_disp"): _zero),
   _permeability_old(declarePropertyOld<Real>("permeability")),
   _aperture(declareProperty<Real>("aperture")) //,
 //  _strain(declareProperty<RealVectorValue>("strain"))

{ }

void
PorousMedia::computeProperties()
{
	Real aperture_old, aperture_change, aperture_new, porosity;
	
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
//rock properties
	  
	  //rkp
	  if(_has_frac_perm)
	  {
		  if (_t_step == 1)
		    {//std::cout << _t_step;
			  _permeability[qp] = _input_permeability;
			  _aperture[qp] = std::sqrt(_input_permeability*12) ;
			}
		  else
		  { 
			  aperture_old = std::sqrt(_permeability_old[qp]*12) ;
			  aperture_change = std::abs(_z_disp[qp]-_z_disp_old[qp]);
			//  _aperture[qp] = std::sqrt(_input_permeability*12) ; 
            //  aperture_change = _strain[_qp](0)+_strain[_qp](1);
              //if (_dim == 3) 
              //{aperture_change += _strain[_qp](2);}
              // _aperture[qp] += (1.0-_aperture[qp]) * aperture_change;
			 // std::cerr << aperture_old <<"  "<< aperture_change<<"  " << aperture_new << ".\n";
              //  porosity =  _input_material_porosity;
              //  _material_porosity[qp]= porosity +  aperture_change*(1.0- porosity);                
              // _permeability[qp] = _permeability[qp] * pow((_material_porosity[qp]/_input_material_porosity),3);	
		  }
	  }
	  
	  //rkp
	  if(_has_frac_perm==false)
	  {
      _permeability[qp]         = _input_permeability;
	  }
	  
    _material_porosity[qp]    = _input_material_porosity;
    _density_rock[qp]         = _input_density_rock;    

// fluid properties
    _compressibility[qp]      = _input_compressibility;  
      
//  gravity    
    _gravity_vector[qp](0) = _gx; 
    _gravity_vector[qp](1) = _gy;
    _gravity_vector[qp](2) = _gz;
    _gravity[qp]           = _input_gravity;
  }
}
