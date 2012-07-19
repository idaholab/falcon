/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "CoupledTemperatureAux.h"

template<>
InputParameters validParams<CoupledTemperatureAux>()
{
    InputParameters params = validParams<AuxKernel>();

    params.addParam<Real>("temperature", 200.,"Use temperature to calculate variable density");
    return params;
}

CoupledTemperatureAux::CoupledTemperatureAux(const std::string & name, InputParameters parameters)
    :AuxKernel(name, parameters),
     _input_temperature(getParam<Real>("temperature"))
{}

Real
CoupledTemperatureAux::computeValue()
{
  return _u[_qp];

}


