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

#include "FracturesGeothermal.h"

template<>
InputParameters validParams<FracturesGeothermal>()
{
  InputParameters params = validParams<FracturesPorousMedia>();
  params += validParams<FracturesFluidFlow>();
  params += validParams<FracturesHeatTransport>();
  params += validParams<FracturesSolidMechanics>();
    
    params.addParam<Real>("fracture_num", 0, "number in fracture map that indicates fractures");
    params.addParam<Real>("matrix_num", 1, "number in fracture map that indicates matrix");
    params.addParam<bool>("has_strain_change_permeability",false,"switch for displacement dependent permeability changes");
    params.addParam<Real>("model_fracture_aperture", 1.0, "width of fracture/high permeability area in the model");
    
  return params;
}

FracturesGeothermal::FracturesGeothermal(const std::string & name,
                       InputParameters parameters)
  :FracturesPorousMedia(name, parameters),
   FracturesFluidFlow(name, parameters),
   FracturesHeatTransport(name, parameters),
   FracturesSolidMechanics(name, parameters),

    _fracture_num(getParam<Real>("fracture_num")),
    _matrix_num(getParam<Real>("matrix_num")),

    _has_strain_change_permeability(getParam<bool>("has_strain_change_permeability")),
    _model_fracture_aperture(getParam<Real>("model_fracture_aperture")),
    _eq_aperture(declareProperty<Real>("eq_aperture"))

{}

void
FracturesGeothermal::computeProperties()
{
  FracturesPorousMedia::computeProperties();
  // Set already computed to true as we compute the intermediate classes' properties
  setPropsComputed(true);

  FracturesFluidFlow::computeProperties();
  FracturesHeatTransport::computeProperties();
  FracturesSolidMechanics::computeProperties();

    for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
    {
        _eq_aperture[qp] = 0.0;
        
        if (_has_strain_change_permeability)
        {
            //////Determining magnitude of aperture change (ie. strain perpandicular to fracture surface)/
        
            //determining direction of fluid flow (which is parallel with fracture direction)
            Real vx = _darcy_flux_water[qp](0);
            Real vy = _darcy_flux_water[qp](1);
                    
            //finding vector perpendicular to fluid flow
            Real ux = std::abs((1/sqrt(vx*vx + vy*vy))*vy);
            Real uy = std::abs((1/sqrt(vx*vx + vy*vy))*vx);
        
            //finding strain vector perpendicular to fluid flow
            Real fracture_strain_normal_x = ux*_strain_normal_vector[qp](0);
            Real fracture_strain_normal_y = uy*_strain_normal_vector[qp](1);
                    
            //magnitude of strain perpandicular to fluid flow
            Real fracture_strain_normal = fracture_strain_normal_x + fracture_strain_normal_y;
            
            if (_t_step == 1)
            {
                if (_fractures[qp] == _matrix_num)
                {
                    _permeability[qp] = _matrix_permeability;
                    _eq_aperture[qp] = 0.0;
                }
                else
                {
                    _permeability[qp] = _fracture_permeability;
                    _eq_aperture[qp] = sqrt(12*_fracture_permeability);
                }
            }
            else
            {
                if (_has_fractures)
                {
                    if (_fractures[qp] == _matrix_num)
                    {
                        _permeability[qp] = _matrix_permeability;
                        _eq_aperture[qp] = 0.0;
                    }
                    else if (_fractures[qp] == _fracture_num)
                    {
                        Real aperture = sqrt(12 * _fracture_permeability);
                        Real fracture_ratio = aperture / _model_fracture_aperture;
                        Real transmissivity_rel = std::pow((1 + (fracture_strain_normal / fracture_ratio)),3);
                        
                        _permeability[qp] = (transmissivity_rel / _model_fracture_aperture) * _fracture_permeability;
                        
                        //Real transmissivity_rel = std::pow((1 + fracture_strain_normal),3);
                        
                        //_permeability[qp] = transmissivity_rel * _fracture_permeability;
                        
                        _eq_aperture[qp] = sqrt(12*_permeability[qp]);
                        
                        if (_permeability[qp] <= (0.9*_fracture_permeability))
                        {
                            _permeability[qp] = 0.9*_fracture_permeability;
                            _eq_aperture[qp] = 0.0;
                        }
                    }
                    else
                    {
                        _permeability[qp] = _matrix_permeability;
                        _eq_aperture[qp] = 0.0;
                    }
                }

            }
        
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    
  // Now reset this parameter
  setPropsComputed(false);
}
