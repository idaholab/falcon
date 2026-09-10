//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PointEnthalpySourceSinkFromFunction.h"
#include "SinglePhaseFluidProperties.h"
#include "Function.h"

registerMooseObject("FalconApp", PointEnthalpySourceSinkFromFunction);

InputParameters
PointEnthalpySourceSinkFromFunction::validParams()
{
  InputParameters params = DiracKernel::validParams();
  params.addRequiredParam<UserObjectName>(
      "SumQuantityUO",
      "User Object of type=PorousFlowSumQuantity in which to place the total "
      "outflow from the line sink for each time step.");
  params.addRequiredParam<FunctionName>("mass_flux_function",
      "The function holding the mass flux at this point in kg/s. Per computeQpResidual, "
      "mass_flux_function >= 0 (paired with temperature_function <= 0) means extraction, and "
      "mass_flux_function <= 0 (paired with temperature_function > 0) means injection -- the "
      "two functions must be coordinated this way or a mooseError is raised.");
  params.addRequiredParam<FunctionName>("temperature_function",
      "The function holding the temperature (K) of the injected fluid. "
      "Function value must be <= 0 if the fluid is extracted (see mass_flux_function)");
  params.addRequiredCoupledVar(
      "pressure", "Pressure used to calculate the injected/extracted fluid enthalpy (measured in Pa)");
  params.addRequiredParam<UserObjectName>(
      "fp",
      "The name of the user object used to calculate the fluid properties of the injected/extracted fluid");
  params.addRequiredParam<Point>("point", "The x,y,z coordinates of the point source");
  params.addClassDescription("Point source/sink that adds or removes heat energy at a variable "
                             "mass flux rate and a variable temperature, switching between "
                             "extraction (using the local solution temperature) and injection "
                             "(using the prescribed temperature_function) based on the sign of "
                             "mass_flux_function");
  return params;
}

PointEnthalpySourceSinkFromFunction::PointEnthalpySourceSinkFromFunction(
    const InputParameters & parameters)
  : DiracKernel(parameters),
    _total_outflow_enthalpy(const_cast<PorousFlowSumQuantity &>(getUserObject<PorousFlowSumQuantity>("SumQuantityUO"))),
    _func_mass_flux(getFunction("mass_flux_function")),
    _func_temperature(getFunction("temperature_function")),
    _pressure(coupledValue("pressure")),
    _temperature(&getMaterialProperty<Real>("PorousFlow_temperature_qp")),
    _fp(getUserObject<SinglePhaseFluidProperties>("fp")),
    _p(getParam<Point>("point")),
    _p_var_num(coupled("pressure"))
{
  // zero the outflow mass
  _total_outflow_enthalpy.zero();
}

void
PointEnthalpySourceSinkFromFunction::addPoints()
{
  _total_outflow_enthalpy.zero();
  addPoint(_p, 0);
}

PointEnthalpySourceSinkFromFunction::FlowMode
PointEnthalpySourceSinkFromFunction::currentFlowMode(Real mass_flux, Real T_input) const
{
  if (mass_flux >= 0 && T_input <= 0)
    return FlowMode::Extraction;
  if (mass_flux <= 0 && T_input > 0)
    return FlowMode::Injection;

  mooseError("The functions of mass flux and temperature are not coordinated for injection and "
             "extraction: at t = ", _t, ", mass_flux_function = ", mass_flux,
             " and temperature_function = ", T_input,
             ". Extraction requires mass_flux >= 0 paired with temperature <= 0 (the local "
             "solution temperature is used instead); injection requires mass_flux <= 0 paired "
             "with temperature > 0.");
}

Real
PointEnthalpySourceSinkFromFunction::computeQpResidual()
{
  //check whether the fluid is injected for extracted from the point
  Real _mass_flux = _func_mass_flux.value(_t, _p);
  Real _T_input = _func_temperature.value(_t, _p);
  Real h = 0;
  if (currentFlowMode(_mass_flux, _T_input) == FlowMode::Extraction)
    h = _fp.h_from_p_T(_pressure[_qp], (*_temperature)[_qp]);
  else
    h = _fp.h_from_p_T(_pressure[_qp], _T_input);
  // The test function is used here to account for all quadrature points
  _total_outflow_enthalpy.add(_test[_i][_qp]*_mass_flux * h * _dt);
  // negative sign means source, while positive sign means sink
  return _test[_i][_qp] * _mass_flux * h;
}

Real
PointEnthalpySourceSinkFromFunction::computeQpJacobian()
{
  // This kernel is applied to the temperature variable. In the extraction branch h depends on
  // PorousFlow_temperature_qp (the solve variable), so there is a genuine dh/dT diagonal term;
  // in the injection branch h depends on _T_input (a prescribed function value), so there isn't.
  Real _mass_flux = _func_mass_flux.value(_t, _p);
  Real _T_input = _func_temperature.value(_t, _p);
  if (currentFlowMode(_mass_flux, _T_input) == FlowMode::Extraction)
  {
    Real h, dh_dp, dh_dT;
    _fp.h_from_p_T(_pressure[_qp], (*_temperature)[_qp], h, dh_dp, dh_dT);
    return _test[_i][_qp] * _phi[_j][_qp] * _mass_flux * dh_dT;
  }

  // Injection: h is evaluated at the prescribed temperature_function value, so it carries no
  // dependence on the temperature solution variable.
  return 0.;
}

Real
PointEnthalpySourceSinkFromFunction::computeQpOffDiagJacobian(unsigned int jvar)
{

  if (jvar == _p_var_num)
  {
    Real h, dh_dp, dh_dT;
    Real _mass_flux = _func_mass_flux.value(_t, _p);
    Real _T_input = _func_temperature.value(_t, _p);
    if (currentFlowMode(_mass_flux, _T_input) == FlowMode::Extraction)
      _fp.h_from_p_T(_pressure[_qp], (*_temperature)[_qp], h, dh_dp, dh_dT);
    else
      _fp.h_from_p_T(_pressure[_qp], _T_input, h, dh_dp, dh_dT);

    return _test[_i][_qp] * _phi[_j][_qp] * _mass_flux * dh_dp;
  }
  else
    return 0.;
}
