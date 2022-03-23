//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PorousFlowReporterPointEnthalpySourcePP.h"
#include "SinglePhaseFluidProperties.h"

registerMooseObject("FalconApp", PorousFlowReporterPointEnthalpySourcePP);

InputParameters
PorousFlowReporterPointEnthalpySourcePP::validParams()
{
  InputParameters params = DiracKernel::validParams();
  params.addRequiredParam<PostprocessorName>(
      "mass_flux",
      "The postprocessor name holding the mass flux of injected fluid at this point in kg/s "
      "(please ensure this is positive so that this object acts like a source)");
  params.addRequiredParam<UserObjectName>(
      "fp",
      "The name of the user object used to calculate the fluid properties of the injected fluid");
  params.addRequiredCoupledVar(
      "pressure", "Pressure used to calculate the injected fluid enthalpy (measured in Pa)");
  params.addRequiredParam<PostprocessorName>(
      "T_in", "The postprocessor name holding the temperature of injected fluid (measured in K)");
  params.addRequiredParam<ReporterName>("x_coord_reporter",
                                        "reporter x-coordinate name of the point source (or sink). "
                                        " This uses the reporter syntax <reporter>/<name>.");
  params.addRequiredParam<ReporterName>("y_coord_reporter",
                                        "reporter y-coordinate name of the point source (or sink). "
                                        " This uses the reporter syntax <reporter>/<name>.");
  params.addRequiredParam<ReporterName>("z_coord_reporter",
                                        "reporter z-coordinate name of the point source (or sink). "
                                        " This uses the reporter syntax <reporter>/<name>.");
  params.addClassDescription("This is a copy of PorousFlowPointEnthalpySourceFromPostProcessor.  "
                             "The only difference is the point location is defined in a reporter.");
  return params;
}

PorousFlowReporterPointEnthalpySourcePP::PorousFlowReporterPointEnthalpySourcePP(
    const InputParameters & parameters)
  : DiracKernel(parameters),
    ReporterInterface(this),
    _mass_flux(getPostprocessorValue("mass_flux")),
    _pressure(coupledValue("pressure")),
    _T_in(getPostprocessorValue("T_in")),
    _fp(getUserObject<SinglePhaseFluidProperties>("fp")),
    _x_coord(getReporterValue<std::vector<Real>>("x_coord_reporter", REPORTER_MODE_REPLICATED)),
    _y_coord(getReporterValue<std::vector<Real>>("y_coord_reporter", REPORTER_MODE_REPLICATED)),
    _z_coord(getReporterValue<std::vector<Real>>("z_coord_reporter", REPORTER_MODE_REPLICATED)),
    _p_var_num(coupled("pressure"))
{
}

void
PorousFlowReporterPointEnthalpySourcePP::addPoints()
{
  if (_x_coord.size() != 1)
  {
    std::string errMsg = "Only a single location can be in this reporter ie a vector of size 1.\n"
                         "x_coord size = " +
                         std::to_string(_x_coord.size()) +
                         ";  y_coord size = " + std::to_string(_y_coord.size()) +
                         ";  z_coord size = " + std::to_string(_z_coord.size());

    mooseError(errMsg);
  }

  Point pt(_x_coord[0], _y_coord[0], _z_coord[0]);
  addPoint(pt, 0);
}

Real
PorousFlowReporterPointEnthalpySourcePP::computeQpResidual()
{
  // Negative sign to make a positive mass_flux in the input file a source
  Real h = _fp.h_from_p_T(_pressure[_qp], _T_in);
  return -_test[_i][_qp] * _mass_flux * h;
}

Real
PorousFlowReporterPointEnthalpySourcePP::computeQpJacobian()
{
  return 0.;
}

Real
PorousFlowReporterPointEnthalpySourcePP::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _p_var_num)
  {
    Real h, dh_dp, dh_dT;
    _fp.h_from_p_T(_pressure[_qp], _T_in, h, dh_dp, dh_dT);
    return -_test[_i][_qp] * _phi[_j][_qp] * _mass_flux * dh_dp;
  }
  else
    return 0.;
}
