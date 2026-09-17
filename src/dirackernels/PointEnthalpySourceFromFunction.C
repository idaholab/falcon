//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PointEnthalpySourceFromFunction.h"
#include "SinglePhaseFluidProperties.h"
#include "Function.h"

registerMooseObject("FalconApp", PointEnthalpySourceFromFunction);

InputParameters
PointEnthalpySourceFromFunction::validParams()
{
  InputParameters params = DiracKernel::validParams();
  params.addRequiredParam<PostprocessorName>(
      "mass_flux",
      "The postprocessor name holding the mass flux of extracted fluid at this point in kg/s "
      "(despite the class name, a positive value acts like a sink: see the residual sign in "
      "computeQpResidual)");
  params.addRequiredParam<UserObjectName>(
      "fp",
      "The name of the user object used to calculate the fluid properties of the extracted fluid");
  params.addRequiredCoupledVar(
      "pressure", "Pressure used to calculate the extracted fluid enthalpy (measured in Pa)");
  params.addRequiredParam<FunctionName>(
      "function", "The function holding the temperature (K) at which the enthalpy of the "
      "extracted fluid is evaluated");
  params.addRequiredParam<Point>("point", "The x,y,z coordinates of the point source");
  params.addClassDescription("Point sink that removes heat energy corresponding to extraction of "
                             "a fluid at a specified mass flux rate (specified by a "
                             "postprocessor), at a temperature given by a function of time and "
                             "position (not the local solution temperature)");
  return params;
}

PointEnthalpySourceFromFunction::PointEnthalpySourceFromFunction(const InputParameters & parameters)
  : DiracKernel(parameters),
    _mass_flux(getPostprocessorValue("mass_flux")),
    _pressure(coupledValue("pressure")),
    _func(getFunction("function")),
    _fp(getUserObject<SinglePhaseFluidProperties>("fp")),
    _p(getParam<Point>("point")),
    _p_var_num(coupled("pressure"))
{
}

void
PointEnthalpySourceFromFunction::addPoints()
{
  addPoint(_p, 0);
}

Real
PointEnthalpySourceFromFunction::computeQpResidual()
{
  Real h = _fp.h_from_p_T(_pressure[_qp], _func.value(_t, _q_point[_qp]));
  return _test[_i][_qp] * _mass_flux * h;
}

Real
PointEnthalpySourceFromFunction::computeQpJacobian()
{
  return 0.;
}

Real
PointEnthalpySourceFromFunction::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _p_var_num)
  {
    Real h, dh_dp, dh_dT;
    _fp.h_from_p_T(_pressure[_qp], _func.value(_t, _q_point[_qp]), h, dh_dp, dh_dT);
    return _test[_i][_qp] * _phi[_j][_qp] * _mass_flux * dh_dp;
  }
  else
    return 0.;
}
