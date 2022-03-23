//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "ReporterVectorComponents.h"
#include "MooseUtils.h"

registerMooseObject("FalconApp", ReporterVectorComponents);

InputParameters
ReporterVectorComponents::validParams()
{
  InputParameters params = GeneralReporter::validParams();
  params.addClassDescription("Reporter to split vector reporters up into scalars");
  params.addRequiredParam<std::vector<ReporterName>>(
      "reporters", "The reporters containing vectors that get split into scalars.");
  params.addRequiredParam<std::vector<unsigned int>>(
      "indices", "Indices of the reporter vectors for which to report a value");
  return params;
}

ReporterVectorComponents::ReporterVectorComponents(const InputParameters & parameters)
  : GeneralReporter(parameters), _vector_indices(getParam<std::vector<unsigned int>>("indices"))
{
}

void
ReporterVectorComponents::initialSetup()
{
  const ReporterData & rdata = _fe_problem.getReporterData();

  for (const auto & rname : getParam<std::vector<ReporterName>>("reporters"))
  {
    if (!rdata.hasReporterValue(rname))
      paramError("reporters", "Reporter ", rname, " does not exist.");

    if (!declareComponentHelper<std::vector<int>>(rname) &&
        !declareComponentHelper<std::vector<Real>>(rname) &&
        !declareComponentHelper<std::vector<std::string>>(rname) &&
        !declareComponentHelper<std::vector<dof_id_type>>(rname))
      paramError("reporters",
                 "VectorToscalar Reporter value ",
                 rname,
                 " is of unsupported type ",
                 rdata.getReporterContextBase(rname).type(),
                 ".");
  }
}

void
ReporterVectorComponents::execute()
{
  for (auto & val : _scalar_values)
    val->writeData();
}
