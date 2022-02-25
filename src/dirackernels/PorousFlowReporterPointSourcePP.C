//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "ReporterPointSource.h"
#include "PorousFlowReporterPointSourcePP.h"

registerMooseObject("FalconApp", PorousFlowReporterPointSourcePP);

InputParameters
PorousFlowReporterPointSourcePP::validParams()
{
  InputParameters params = DiracKernel::validParams();
  params.addRequiredParam<PostprocessorName>(
      "mass_flux",
      "The postprocessor name holding the mass flux at this point in kg/s (positive is flux in, "
      "negative is flux out)");
  params.addRequiredParam<ReporterName>("x_coord_reporter",
                                        "reporter x-coordinate name of the point source (or sink). "
                                        " This uses the reporter syntax <reporter>/<name>.");
  params.addRequiredParam<ReporterName>("y_coord_reporter",
                                        "reporter y-coordinate name of the point source (or sink). "
                                        " This uses the reporter syntax <reporter>/<name>.");
  params.addRequiredParam<ReporterName>("z_coord_reporter",
                                        "reporter z-coordinate name of the point source (or sink). "
                                        " This uses the reporter syntax <reporter>/<name>.");

  params.addClassDescription("This is a copy of "
                             "PorousFlowPointSourceFromPostProcessor.  The only difference is the "
                             "point location is defined in a reporter");
  return params;
}

PorousFlowReporterPointSourcePP::PorousFlowReporterPointSourcePP(const InputParameters & parameters)
  : DiracKernel(parameters),
    ReporterInterface(this),
    _mass_flux(getPostprocessorValue("mass_flux")),
    _x_coord(getReporterValue<std::vector<Real>>("x_coord_reporter", REPORTER_MODE_REPLICATED)),
    _y_coord(getReporterValue<std::vector<Real>>("y_coord_reporter", REPORTER_MODE_REPLICATED)),
    _z_coord(getReporterValue<std::vector<Real>>("z_coord_reporter", REPORTER_MODE_REPLICATED))
{
}

void
PorousFlowReporterPointSourcePP::addPoints()
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
PorousFlowReporterPointSourcePP::computeQpResidual()
{
  // Negative sign to make a positive mass_flux in the input file a source
  return -_test[_i][_qp] * _mass_flux;
}
