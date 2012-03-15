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

#include "TemperatureTimeDerivativeSolid.h"
#include "Material.h"

template<>
InputParameters validParams<TemperatureTimeDerivativeSolid>()
{
  InputParameters params = validParams<TimeDerivative>();
//  params.addRequiredCoupledVar("density_water", "Use CoupledAuxDensity here");
  params.addRequiredCoupledVar("porosity", "Use CoupledAuxPorosity here");
  return params;
}

TemperatureTimeDerivativeSolid::TemperatureTimeDerivativeSolid(const std::string & name,
                                                     InputParameters parameters)
  :TimeDerivative(name, parameters),


//   _density_water(coupledValue("density_water")),
//   _density_water_old(coupledValueOld("density_water")),
   
   _porosity(coupledValue("porosity")),
   _porosity_old(coupledValueOld("porosity")),

//   _specific_heat_water(getMaterialProperty<Real>("specific_heat_water")),
   _specific_heat_rock(getMaterialProperty<Real>("specific_heat_rock")),
   _density_rock(getMaterialProperty<Real>("density_rock")),
   _u_old(valueOld())
{}

Real
TemperatureTimeDerivativeSolid::computeQpResidual()
{
  
  //REAL dphirho_dt = ((_porosity[_qp]*_density_water[_qp])-(_porosity_old[_qp]*_density_water_old[_qp]))/_dt;
  
  return
    (_density_rock[_qp]*_specific_heat_rock[_qp])
    *
    ((((1-_porosity[_qp])*_u[_qp]) - ((1-_porosity_old[_qp])*_u_old[_qp]))/_dt)
    *
    _test[_i][_qp];
}

Real
TemperatureTimeDerivativeSolid::computeQpJacobian()
{
  return 0;
}
