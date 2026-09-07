//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowSquarePulsePointEnthalpySink.h"
#include "SinglePhaseFluidProperties.h"

registerMooseObject("FalconApp", PorousFlowSquarePulsePointEnthalpySink);

InputParameters
PorousFlowSquarePulsePointEnthalpySink::validParams()
{
  InputParameters params = DiracKernel::validParams();
  params.addRequiredParam<Real>(
      "mass_flux",
      "The mass flux of extracted fluid at this point in kg/s, active between start_time and "
      "end_time (positive acts as a sink; see the residual sign in computeQpResidual)");
  params.addRequiredParam<UserObjectName>(
      "fp",
      "The name of the user object used to calculate the fluid properties of the extracted fluid");
  params.addRequiredCoupledVar(
      "pressure", "Pressure used to calculate the extracted fluid enthalpy (measured in Pa)");
  params.addRequiredParam<Point>("point", "The x,y,z coordinates of the point source");
  params.addParam<Real>(
      "start_time", 0.0, "The time at which the source will start (Default is 0)");
  params.addParam<Real>(
      "end_time", 1.0e30, "The time at which the source will end (Default is 1e30)");
  params.addClassDescription("Point sink that removes heat energy at a constant mass flux rate "
                             "for times between start_time and end_time, at the LOCAL solution "
                             "temperature (PorousFlow_temperature_qp material property), not a "
                             "prescribed value");
  return params;
}

PorousFlowSquarePulsePointEnthalpySink::PorousFlowSquarePulsePointEnthalpySink(
    const InputParameters & parameters)
  : DiracKernel(parameters),
    _mass_flux(getParam<Real>("mass_flux")),
    _pressure(coupledValue("pressure")),
    _temperature(&getMaterialProperty<Real>("PorousFlow_temperature_qp")),
    _fp(getUserObject<SinglePhaseFluidProperties>("fp")),
    _p(getParam<Point>("point")),
    _start_time(getParam<Real>("start_time")),
    _end_time(getParam<Real>("end_time")),
    _p_var_num(coupled("pressure"))
{
    // Sanity check to ensure that the end_time is greater than the start_time
  if (_end_time <= _start_time)
    mooseError("start_time is ", _start_time, " but it must be less than end_time ", _end_time);
}

void
PorousFlowSquarePulsePointEnthalpySink::addPoints()
{
  addPoint(_p, 0);
}

Real
PorousFlowSquarePulsePointEnthalpySink::computeQpResidual()
{
   Real factor = 0.0;
    /**
   * There are six cases for the start and end time in relation to t-dt and t.
   * If the interval (t-dt,t) is only partly but not fully within the (start,end)
   * interval, then the  mass_flux is scaled so that the total mass added
   * (or removed) is correct
   */
  if (_t < _start_time || _t - _dt >= _end_time)
    factor = 0.0;
  else if (_t - _dt < _start_time)
  {
    if (_t <= _end_time)
      factor = (_t - _start_time) / _dt;
    else
      factor = (_end_time - _start_time) / _dt;
  }
  else
  {
    if (_t <= _end_time)
      factor = 1.0;
    else
      factor = (_end_time - (_t - _dt)) / _dt;
  }
  // No leading minus: a positive mass_flux acts as a SINK, removing energy from the domain.
  Real h = _fp.h_from_p_T(_pressure[_qp], (*_temperature)[_qp]);
  return _test[_i][_qp] * factor * _mass_flux * h;
}

Real
PorousFlowSquarePulsePointEnthalpySink::computeQpJacobian()
{
  // This kernel is applied to the temperature variable, and h depends on
  // PorousFlow_temperature_qp, so there is a genuine dh/dT diagonal term.
  Real factor = 0.0;

  /**
   * There are six cases for the start and end time in relation to t-dt and t.
   * If the interval (t-dt,t) is only partly but not fully within the (start,end)
   * interval, then the  mass_flux is scaled so that the total mass added
   * (or removed) is correct
   */
  if (_t < _start_time || _t - _dt >= _end_time)
    factor = 0.0;
  else if (_t - _dt < _start_time)
  {
    if (_t <= _end_time)
      factor = (_t - _start_time) / _dt;
    else
      factor = (_end_time - _start_time) / _dt;
  }
  else
  {
    if (_t <= _end_time)
      factor = 1.0;
    else
      factor = (_end_time - (_t - _dt)) / _dt;
  }

  Real h, dh_dp, dh_dT;
  _fp.h_from_p_T(_pressure[_qp], (*_temperature)[_qp], h, dh_dp, dh_dT);
  return _test[_i][_qp] * _phi[_j][_qp] * factor * _mass_flux * dh_dT;
}

Real
PorousFlowSquarePulsePointEnthalpySink::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real factor = 0.0;

  /**
   * There are six cases for the start and end time in relation to t-dt and t.
   * If the interval (t-dt,t) is only partly but not fully within the (start,end)
   * interval, then the  mass_flux is scaled so that the total mass added
   * (or removed) is correct
   */
  if (_t < _start_time || _t - _dt >= _end_time)
    factor = 0.0;
  else if (_t - _dt < _start_time)
  {
    if (_t <= _end_time)
      factor = (_t - _start_time) / _dt;
    else
      factor = (_end_time - _start_time) / _dt;
  }
  else
  {
    if (_t <= _end_time)
      factor = 1.0;
    else
      factor = (_end_time - (_t - _dt)) / _dt;
  }
  if (jvar == _p_var_num)
  {
    Real h, dh_dp, dh_dT;
    _fp.h_from_p_T(_pressure[_qp], (*_temperature)[_qp], h, dh_dp, dh_dT);
    return _test[_i][_qp] * _phi[_j][_qp]* factor  * _mass_flux * dh_dp;
  }
  else
    return 0.;
}
