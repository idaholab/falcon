/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c)     2012 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "MooseError.h"
#include "CSVNodalMaterialDataAux.h"

registerMooseObject("FalconApp", CSVNodalMaterialDataAux);

template <>
InputParameters
validParams<CSVNodalMaterialDataAux>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addRequiredParam<VectorPostprocessorName>(
      "csv_reader_object_name",
      "The vector posptorcessor to examine.");

  params.addRequiredParam<std::string>(
      "attribute_name",
      "The vector to consider from the VectorPosptorcessor.");

  return params;
}

CSVNodalMaterialDataAux::CSVNodalMaterialDataAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _vpp_data(getVectorPostprocessorValue("csv_reader_object_name", getParam<std::string>("attribute_name")))
{
}

Real
CSVNodalMaterialDataAux::computeValue()
{
  // The value to output
  Real output;

  if (isNodal())
  {
    if (_current_node->id() >= _vpp_data.size())
    {
      mooseError("The total number of rows in CSV file = ", _vpp_data.size(),
                 " is smaller than the current node id = ", _current_node->id());
    }
    output = _vpp_data[_current_node->id()];
  }
  else
    mooseError("Must use nodal aux variable!");

  return output;
}
