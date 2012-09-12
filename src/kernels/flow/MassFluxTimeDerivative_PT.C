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

#include "MassFluxTimeDerivative_PT.h"
#include "Material.h"

template<>
InputParameters validParams<MassFluxTimeDerivative_PT>()
{
  InputParameters params = validParams<TimeDerivative>();
   //params.addRequiredCoupledVar("density_water", "Use Coupled density here to calculate the time derivative");
   //params.addRequiredCoupledVar("dwdp", "derivative of water density vs temperature");
    return params;
}

MassFluxTimeDerivative_PT::MassFluxTimeDerivative_PT(const std::string & name, InputParameters parameters)
  :TimeDerivative(name, parameters),
   //_density_water(coupledValue("density_water")),                 //removed by kat
   //_density_water_old(coupledValueOld("density_water")),          //      |
   //_dwdp(coupledValue("dwdp")),                                   //      V
   _density_water(getMaterialProperty<Real>("density_water")),
   _density_water_old(getMaterialProperty<Real>("time_old_density_water")),
   _dwdp(getMaterialProperty<Real>("dwdp")),
   _porosity (getMaterialProperty<Real>("material_porosity"))
{}

Real
MassFluxTimeDerivative_PT::computeQpResidual()
{
  return (((_porosity[_qp]*_density_water[_qp])-(_porosity[_qp]*_density_water_old[_qp]))/_dt) * _test[_i][_qp];
}

Real
MassFluxTimeDerivative_PT::computeQpJacobian()
{
  Real tmp1 = (_porosity[_qp]*_dwdp[_qp]*_phi[_j][_qp])*_test[_i][_qp]/_dt;  
  return tmp1;
}
