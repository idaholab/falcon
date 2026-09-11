//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PointEnthalpySink.h"
#include "SinglePhaseFluidProperties.h"

registerMooseObject("FalconApp", PointEnthalpySink);

InputParameters
PointEnthalpySink::validParams()
{
  InputParameters params = DiracKernel::validParams();
  params.addRequiredParam<PostprocessorName>(
      "mass_flux",
      "The postprocessor name holding the mass flux of extracted fluid at this point in kg/s "
      "(please ensure this is positive so that this object acts like a sink)");
  params.addRequiredParam<UserObjectName>(
      "fp",
      "The name of the user object used to calculate the fluid properties of the injected fluid");
  params.addRequiredCoupledVar(
      "pressure", "Pressure used to calculate the injected fluid enthalpy (measured in Pa)");
  params.addRequiredParam<Point>("point", "The x,y,z coordinates of the point source");
  params.addClassDescription("Point sink that removes heat energy corresponding to extraction of "
                             "a fluid at a specified mass flux rate (specified by a "
                             "postprocessor), at the LOCAL solution temperature "
                             "(PorousFlow_temperature_qp material property), not a fixed or "
                             "prescribed value");
  return params;
}

PointEnthalpySink::PointEnthalpySink(
    const InputParameters & parameters)
  : DiracKernel(parameters),
    _mass_flux(getPostprocessorValue("mass_flux")),
    _pressure(coupledValue("pressure")),
    _temperature(&getMaterialProperty<Real>("PorousFlow_temperature_qp")),
    _fp(getUserObject<SinglePhaseFluidProperties>("fp")),
    _p(getParam<Point>("point")),
    _p_var_num(coupled("pressure"))
{
}

void
PointEnthalpySink::addPoints()
{
  addPoint(_p, 0);
}

Real
PointEnthalpySink::computeQpResidual()
{
  // No leading minus: a positive mass_flux acts as a SINK, removing energy from the domain.
  Real h = _fp.h_from_p_T(_pressure[_qp], (*_temperature)[_qp]);
  return _test[_i][_qp] * _mass_flux * h;
}

Real
PointEnthalpySink::computeQpJacobian()
{
  // This kernel is applied to the temperature variable, and h depends on
  // PorousFlow_temperature_qp, so there is a genuine dh/dT diagonal term.
  Real h, dh_dp, dh_dT;
  _fp.h_from_p_T(_pressure[_qp], (*_temperature)[_qp], h, dh_dp, dh_dT);
  return _test[_i][_qp] * _phi[_j][_qp] * _mass_flux * dh_dT;
}

Real
PointEnthalpySink::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _p_var_num)
  {
    Real h, dh_dp, dh_dT;
    _fp.h_from_p_T(_pressure[_qp], (*_temperature)[_qp], h, dh_dp, dh_dT);
    return _test[_i][_qp] * _phi[_j][_qp] * _mass_flux * dh_dp;
  }
  else
    return 0.;
}
