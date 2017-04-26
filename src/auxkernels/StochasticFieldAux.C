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

#include "StochasticFieldAux.h"

template<>
InputParameters validParams<StochasticFieldAux>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addRequiredParam<std::string>(
  "file_name",
  "The name of the file that contains one stochastic parameter values");

  return params;
}


StochasticFieldAux::StochasticFieldAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _stoch_field(getParam<std::string>("file_name"))
{
}

Real
StochasticFieldAux::computeValue()
{
  return _stoch_field.value(_current_elem->centroid());
}
