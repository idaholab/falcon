//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "GeneralReporter.h"

class ComponentValueBase;
template <typename T>
class ComponentValue;

class ReporterVectorComponents : public GeneralReporter
{
public:
  static InputParameters validParams();
  ReporterVectorComponents(const InputParameters & parameters);
  virtual void initialSetup() override;
  virtual void initialize() override {}
  virtual void execute() override;
  virtual void finalize() override {}

protected:
  /**
   * Helper for declaring an scalar reporter value
   * This will fill in _scalar_values if the reporter value is found
   */
  template <typename T>
  bool declareComponentHelper(const ReporterName & rname);

  /// Vector of scalar value objects
  std::vector<std::unique_ptr<ComponentValueBase>> _scalar_values;

private:
  const std::vector<unsigned int> _vector_indices;
};

template <typename T>
bool
ReporterVectorComponents::declareComponentHelper(const ReporterName & rname)
{
  const ReporterData & rdata = _fe_problem.getReporterData();

  if (!rdata.hasReporterValue<T>(rname))
    return false;

  const auto & pmode = rdata.getReporterMode(rname);
  ReporterMode rmode = REPORTER_MODE_UNSET;
  if (pmode == REPORTER_MODE_ROOT)
    rmode = REPORTER_MODE_ROOT;
  else if (pmode == REPORTER_MODE_REPLICATED)
    rmode = REPORTER_MODE_REPLICATED;
  else if (pmode == REPORTER_MODE_DISTRIBUTED)
    rmode = REPORTER_MODE_DISTRIBUTED;

  const T & val = getReporterValueByName<T>(rname);

  for (auto & id : _vector_indices)
  {

    std::cout << "rname.getObjectName():  " << rname.getObjectName()
              << "    rname.getValueName(): " << rname.getValueName() << std::endl;

    typename T::value_type & scalar_val = declareValueByName<typename T::value_type>(
        rname.getObjectName() + ":" + rname.getValueName() + "[" + std::to_string(id) + "]", rmode);

    _scalar_values.push_back(
        std::make_unique<ComponentValue<typename T::value_type>>(val, scalar_val, id));
  }

  return true;
}

class ComponentValueBase
{
public:
  virtual ~ComponentValueBase() = default;
  virtual void writeData() = 0;
};

template <typename S>
class ComponentValue : public ComponentValueBase
{
public:
  ComponentValue(const std::vector<S> & val, S & scalar_val, unsigned int val_index)
    : ComponentValueBase(), _val(val), _scalar_val(scalar_val), _val_index(val_index)
  {
  }

  virtual void writeData() override
  {
    if (_val_index >= _val.size())
      mooseError(
          "In VectorReporterComponent index greater than size of vector reporter being read from.\n"
          "index = ",
          _val_index,
          "    vector_size = ",
          _val.size());

    _scalar_val = _val[_val_index];
  }

private:
  const std::vector<S> & _val;
  S & _scalar_val;
  unsigned int _val_index;
};
