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
      "The function holding the mass flux at this point in kg/s (positive is flux in,"
      "negative is flux out)");
  params.addRequiredParam<FunctionName>("temperature_function", 
      "The function holding the temperature (K) of the injected fluid,"
      "Function value is 0 if the fluid is extracted"); 
  params.addRequiredCoupledVar(
      "pressure", "Pressure used to calculate the injected fluid enthalpy (measured in Pa)");
  params.addRequiredParam<UserObjectName>(
      "fp",
      "The name of the user object used to calculate the fluid properties of the injected fluid");
  params.addRequiredParam<Point>("point", "The x,y,z coordinates of the point source");
  params.addClassDescription("Integrated Point Sink that adds/minus heat energy at a variable mass "
                             " flux rate and a variable temperature ");
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

Real
PointEnthalpySourceSinkFromFunction::computeQpResidual()
{
  //check whether the fluid is injected for extracted from the point
  Real _mass_flux = _func_mass_flux.value(_t, _q_point[_qp]);
  Real _T_input = _func_temperature.value(_t, _q_point[_qp]);
  // printf("_mass_flux=%f",_mass_flux);
  // printf("_T_input=%f",_T_input);
  Real h =0;
  if ((_mass_flux>=0) && (_T_input<=0)){ //extraction
    h = _fp.h_from_p_T(_pressure[_qp], (*_temperature)[_qp]);
    // printf("check_extraction");
  }else if((_mass_flux<=0) && (_T_input>0)){ //injection
    h = _fp.h_from_p_T(_pressure[_qp], _T_input);
    // printf("check_injection");
  }else{
    mooseError(name(), "The functions of mass flux and temperature are not coordinated for injection and extraction");
  }
  _total_outflow_enthalpy.add(_mass_flux * h * _dt);
  // negative sign means source, while positive sign means sink
  return _test[_i][_qp] * _mass_flux * h;
}

Real
PointEnthalpySourceSinkFromFunction::computeQpJacobian()
{
  return 0.;
}

Real
PointEnthalpySourceSinkFromFunction::computeQpOffDiagJacobian(unsigned int jvar)
{

  if (jvar == _p_var_num)
  {
    Real h, dh_dp, dh_dT;
    Real _mass_flux = _func_mass_flux.value(_t, _q_point[_qp]);
    Real _T_input = _func_temperature.value(_t, _q_point[_qp]);
    if ((_mass_flux>=0) && (_T_input<=0)){ //extraction
      _fp.h_from_p_T(_pressure[_qp], (*_temperature)[_qp], h, dh_dp, dh_dT);
      return -_test[_i][_qp] * _phi[_j][_qp]* _mass_flux * dh_dp;
    }else if((_mass_flux<=0) && (_T_input>0)){ //injection
      _fp.h_from_p_T(_pressure[_qp], _T_input, h, dh_dp, dh_dT);
      return -_test[_i][_qp] * _phi[_j][_qp]* _mass_flux * dh_dp;
    }else{
    mooseError(name(), "The functions of mass flux and temperature are not coordinated for injection and extraction");
    }
  }
  else
    return 0.;
}