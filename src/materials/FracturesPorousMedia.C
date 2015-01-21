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
  params.addParam<Real>("matrix_permeability",1.0e-12, "intrinsic permeability of matrix (m^2)");
  params.addParam<Real>("matrix_porosity", 0.3, "rock porosity of matrix");
  params.addParam<Real>("matrix_density", 2.50e3, "rock density of matrix (kg/m^3)");

////Gravity
  params.addParam<Real>("gravity",9.80665,"gravity acceleration constant");
  params.addParam<Real>("gx",0.0,"x component of the gravity pressure vector");
  params.addParam<Real>("gy",0.0,"y component of the gravity pressure vector");
  params.addParam<Real>("gz",1.0,"z component of the gravity pressure vector");

////Fractures
  //fracture map/number inputs
  params.addRequiredCoupledVar("fractures", "coupled aux variable that maps where the fracture are");
  params.addParam<Real>("fracture_num", 0, "number in fracture map that indicates fractures");
  params.addParam<Real>("model_fracture_aperture", 1.0, "width of fracture/high permeability area in the model");
  //rock property inputs
  params.addParam<Real>("fracture_permeability", 1e-12, "intrinsic permeability of fractures (m^2)");
  params.addParam<Real>("fracture_porosity", 0.3, "rock porosity of fractures");
  params.addParam<Real>("fracture_density", 2500, "rock density of fractures (kg/m^3)");
  params.addParam<bool>("has_strain_dependent_permeability",false,"switch for displacement dependent permeability changes");

  //flag if chemical reactions are present.  determines whether porosity_old is called
  params.addParam<bool>("has_chem_reactions", false, "add discription");

  return params;
}

FracturesPorousMedia::FracturesPorousMedia(const std::string & name,
                         InputParameters parameters)
  :Material(name, parameters),
////Grab user input parameters
    //Matrix
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
    _has_fractures(isCoupled("fractures")),
    _fractures(_has_fractures ? coupledValue("fractures") : _zero),
    _fracture_num(getParam<Real>("fracture_num")),
    _model_fracture_aperture(getParam<Real>("model_fracture_aperture")),
    //rock property inputs
    _fracture_permeability(getParam<Real>("fracture_permeability")),
    _fracture_porosity(getParam<Real>("fracture_porosity")),
    _fracture_density(getParam<Real>("fracture_density")),

////Delcare material properties
    //rock material props
    _permeability(declareProperty<Real>("permeability")),
    //do we have strain dependent permeability? then we need to declare permeability_old
    _has_strain_dependent_permeability(getParam<bool>("has_strain_dependent_permeability")),
    _permeability_old(_has_strain_dependent_permeability ? &declarePropertyOld<Real>("permeability") : NULL),
    _porosity(declareProperty<Real>("porosity")),
    //do we have chemical reactions happening? then we need to declare porosity_old
    _has_chem_reactions(getParam<bool>("has_chem_reactions")),
    _porosity_old(_has_chem_reactions ? &declarePropertyOld<Real>("porosity") : NULL),
    _density_rock(declareProperty<Real>("density_rock")),
    //gravity material props
    _gravity(declareProperty<Real>("gravity")),
    _gravity_vector(declareProperty<RealVectorValue>("gravity_vector")),

    _already_computed(false)

{}

void
FracturesPorousMedia::initQpStatefulProperties()
{
    if (_fractures[_qp] == _fracture_num)
        _porosity[_qp] = _fracture_porosity;
    else
        _porosity[_qp] = _matrix_porosity;
}

void
FracturesPorousMedia::computeProperties()
{
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
      //material property assignment for fractures
      if (_fractures[qp] == _fracture_num)
      {
          Real aperture = sqrt(12 * _fracture_permeability);
          Real fracture_ratio = _model_fracture_aperture / aperture;

          if (_has_strain_dependent_permeability && (_t_step > 1))
          {
              _permeability[qp] = (*_permeability_old)[qp];
          }
          else
              _permeability[qp] = ((std::pow(_model_fracture_aperture,2)) * (std::pow((1/fracture_ratio) , 3)))/12;

          _porosity[qp]             = _fracture_porosity;
          _density_rock[qp]         = _fracture_density;

      }
      //material property assignment for anything else will get lumped in with the matrix
      else
      {
          _permeability[qp]         = _matrix_permeability;
          _porosity[qp]             = _matrix_porosity;
          _density_rock[qp]         = _matrix_density;
      }

      _gravity_vector[qp](0) = _gx;
      _gravity_vector[qp](1) = _gy;
      _gravity_vector[qp](2) = _gz;
      _gravity[qp]           = _input_gravity;
  }
}
