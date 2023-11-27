//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "LineSourceSinkFromFunction.h"
#include "Function.h"

registerMooseObject("FalconApp", LineSourceSinkFromFunction);

InputParameters
LineSourceSinkFromFunction::validParams()
{
  InputParameters params = PorousFlowLineSink::validParams();
  params.addRequiredParam<FunctionName>("mass_flux_function", "The function holding the mass flux at this point in kg/s (positive is flux in, "
      "negative is flux out)");
  params.addClassDescription(
      "Approximates a polyline sink by using a number of point sinks with "
      "given weighting whose positions are read from a file.  NOTE: if you are using "
      "PorousFlowPorosity that depends on volumetric strain, you should set "
      "strain_at_nearest_qp=true in your GlobalParams, to ensure the nodal Porosity Material uses "
      "the volumetric strain at the Dirac quadpoints, and can therefore be computed");
  return params;
}

LineSourceSinkFromFunction::LineSourceSinkFromFunction(const InputParameters & parameters)
  : PorousFlowLineSink(parameters),
   _func(getFunction("mass_flux_function"))
{
}

Real
LineSourceSinkFromFunction::computeQpBaseOutflow(unsigned current_dirac_ptid) const
{
  Real outflow = 0.0;

  if (current_dirac_ptid > 0)
    // contribution from half-segment "behind" this point (must have >1 point for
    // current_dirac_ptid>0)
    outflow += _half_seg_len[current_dirac_ptid - 1];

  if (current_dirac_ptid + 1 < _zs.size() || _zs.size() == 1)
    // contribution from half-segment "ahead of" this point, or we only have one point
    outflow += _half_seg_len[current_dirac_ptid];

  return outflow * _test[_i][_qp] * _func.value(_t, _q_point[_qp]) * _weight->at(current_dirac_ptid);
}

void
LineSourceSinkFromFunction::computeQpBaseOutflowJacobian(unsigned jvar,
                                                     unsigned current_dirac_ptid,
                                                     Real & outflow,
                                                     Real & outflowp) const
{
  outflow = 0.0;
  outflowp = 0.0;
  if (_dictator.notPorousFlowVariable(jvar))
    return;
  const unsigned pvar = _dictator.porousFlowVariableNum(jvar);

  if (current_dirac_ptid > 0)
    // contribution from half-segment "behind" this point (must have >1 point for
    // current_dirac_ptid>0)
    outflow += _half_seg_len[current_dirac_ptid - 1];

  if (current_dirac_ptid + 1 < _zs.size() || _zs.size() == 1)
    // contribution from half-segment "ahead of" this point, or we only have one point
    outflow += _half_seg_len[current_dirac_ptid];

  outflowp = outflow * _test[_i][_qp] * _phi[_j][_qp] * _weight->at(current_dirac_ptid);

//   outflowp = outflow * _test[_i][_qp] * _phi[_j][_qp] * _weight->at(current_dirac_ptid);
  outflow *= _test[_i][_qp] *_func.value(_t, _q_point[_qp])* _weight->at(current_dirac_ptid);
}
