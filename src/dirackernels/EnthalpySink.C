//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "EnthalpySink.h"
#include "SinglePhaseFluidProperties.h"

registerMooseObject("App", EnthalpySink);

InputParameters
EnthalpySink::validParams()
{
  InputParameters params = PorousFlowPolyLineSink::validParams();
  params.addRequiredParam<UserObjectName>("fp", "The name of the user object for fluid properties");
  params.addRequiredCoupledVar("pressure", "Pressure");
  params.addRequiredParam<Real>("T_in", "The incoming fluid temperature");
  return params;
}

EnthalpySink::EnthalpySink(const InputParameters & parameters)
  : PorousFlowPolyLineSink(parameters),
    _pressure(coupledValue("pressure")),
    _T_in(getParam<Real>("T_in")),
    _fp(getUserObject<SinglePhaseFluidProperties>("fp"))
{
}

Real
EnthalpySink::computeQpBaseOutflow(unsigned current_dirac_ptid) const
{
  Real h = _fp.h_from_p_T(_pressure[_qp], _T_in);
  return PorousFlowPolyLineSink::computeQpBaseOutflow(current_dirac_ptid) * h;
}
