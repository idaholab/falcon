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

#include "Constant.h"

template<>
InputParameters validParams<Constant>()
{
  InputParameters params = validParams<Material>();
  //coupled variables
  params.addCoupledVar("temperature", "Coupled non-linear termperature variable, [K]");
    
  //rock property inputs
  params.addParam<Real>("thermal_conductivity", 1.0, "thermal conductivity coefficient, [W/(m.K)]");
  params.addParam<Real>("thermal_expansion", 1.0, "thermal expansion coefficient, [1/K]");
  params.addParam<Real>("specific_heat", 1.0, "specific heat of material, [J/(kg.K)]");
  params.addParam<Real>("density", 1.0, "density of the water, [kg/m^3]");
  params.addParam<Real>("density_rock", 1.0, "density of the rock matrix, [kg/m^3]");
  params.addParam<Real>("youngs_modulus", 1.0, "youngs modulus, [Pa]");
  params.addParam<Real>("poissons_ratio", 1.0, "Dimentionless");
  params.addParam<Real>("biot_coeff", 1.0, "Dimentionless");
  params.addParam<Real>("thermal_strain_ref_temp", 300.0, "Initial reference temperature where there is no thermal strain, [K]");

  return params;
}

Constant::Constant(const std::string & name,
                   InputParameters parameters)
  :Material(name, parameters),
////Grab coupled variables
     _has_temp(isCoupled("temp")),
     _temp(_has_temp ? coupledValue("temp") : _zero),

////Grab user input parameters
     _my_thermal_conductivity(getParam<Real>("thermal_conductivity")),
     _my_thermal_expansion(getParam<Real>("thermal_expansion")),
     _my_specific_heat(getParam<Real>("specific_heat")),
     _my_density(getParam<Real>("density")),
     _my_youngs_modulus(getParam<Real>("youngs_modulus")),
     _my_poissons_ratio(getParam<Real>("poissons_ratio")),
     _my_biot_coeff(getParam<Real>("biot_coeff")),     
     _my_t_ref(getParam<Real>("thermal_strain_ref_temp")),

////Declare material properties
     _thermal_conductivity(declareProperty<Real>("thermal_conductivity")),
     _thermal_strain(declareProperty<Real>("thermal_strain")),
     _alpha(declareProperty<Real>("alpha")),
     _biot_coeff(declareProperty<Real>("biot_coeff")),
     _specific_heat(declareProperty<Real>("specific_heat")),
     _density(declareProperty<Real>("density")),
     _density_rock(declareProperty<Real>("density_rock")),
     _youngs_modulus(declareProperty<Real>("youngs_modulus")),
     _poissons_ratio(declareProperty<Real>("poissons_ratio"))
  {}

void
Constant::computeProperties()
{
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
    _density[qp] = _my_density;
    _density_rock[qp] = _my_density;
    _thermal_conductivity[qp] = _my_thermal_conductivity;
    _specific_heat[qp] = _my_specific_heat;
    _biot_coeff[qp] = _my_biot_coeff;    

    if(_has_temp)
      _thermal_strain[qp] = _my_thermal_expansion*(_temp[qp] - _my_t_ref);
    else
      _thermal_strain[qp] = 0;

    _alpha[qp] = _my_thermal_expansion;
    
    _youngs_modulus[qp]  = _my_youngs_modulus;
    _poissons_ratio[qp] = _my_poissons_ratio;
  }
}
