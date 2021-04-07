//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "FunctionEnthalpySink.h"
#include "SinglePhaseFluidProperties.h"
#include "Function.h"

registerMooseObject("FalconApp", FunctionEnthalpySink);

InputParameters
FunctionEnthalpySink::validParams()
{
  InputParameters params = PorousFlowPolyLineSink::validParams();
  params.addRequiredParam<UserObjectName>("fp", "The name of the user object for fluid properties");
  params.addRequiredCoupledVar("pressure", "Pressure");
  params.addRequiredParam<FunctionName>("function", "The forcing function.");
  return params;
}

FunctionEnthalpySink::FunctionEnthalpySink(const InputParameters & parameters)
  : PorousFlowPolyLineSink(parameters),
    _pressure(coupledValue("pressure")),
    _func(getFunction("function")),
    _fp(getUserObject<SinglePhaseFluidProperties>("fp"))
{
}

Real
FunctionEnthalpySink::computeQpBaseOutflow(unsigned current_dirac_ptid) const
{
  Real _T_in = _func.value(_t, _q_point[_qp]);
  Real h = _fp.h_from_p_T(_pressure[_qp], _T_in);
  return PorousFlowPolyLineSink::computeQpBaseOutflow(current_dirac_ptid) * h;
}