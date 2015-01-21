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

#include "TemperatureTimeDerivativeFluid.h"
#include "Material.h"

template<>
InputParameters validParams<TemperatureTimeDerivativeFluid>()
{
  InputParameters params = validParams<TimeDerivative>();
  params.addRequiredCoupledVar("porosity", "Use CoupledAuxPorosity here");
  return params;
}

TemperatureTimeDerivativeFluid::TemperatureTimeDerivativeFluid(const std::string & name,
                                                     InputParameters parameters)
  :TimeDerivative(name, parameters),

   _density_water(getMaterialProperty<Real>("density_water")),
   _density_water_old(getMaterialProperty<Real>("time_old_density")),

   _porosity(coupledValue("porosity")),
   _porosity_old(coupledValueOld("porosity")),

   _specific_heat_water(getMaterialProperty<Real>("specific_heat_water")),
   //  _specific_heat_rock(getMaterialProperty<Real>("specific_heat_rock")),
//   _density_rock(getMaterialProperty<Real>("density_rock"))
   _u_old(valueOld())
{}

Real
TemperatureTimeDerivativeFluid::computeQpResidual()
{

  //REAL dphirho_dt = ((_porosity[_qp]*_density_water[_qp])-(_porosity_old[_qp]*_density_water_old[_qp]))/_dt;

  return
    _specific_heat_water[_qp]
    *
    (((_porosity[_qp]*_density_water[_qp]*_u[_qp])-(_porosity_old[_qp]*_density_water_old[_qp]*_u_old[_qp]))/_dt)
    *
    _test[_i][_qp];
}

Real
TemperatureTimeDerivativeFluid::computeQpJacobian()

//need to verify this is the correct Jacobian, RKP 4/19/11
{
  return _specific_heat_water[_qp]*_porosity[_qp] *_density_water[_qp] *(1/_dt) *_phi[_j][_qp];
}
