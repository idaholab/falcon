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

#include "CoupledDensityAux.h"

template<>
InputParameters validParams<CoupledDensityAux>()
{
    InputParameters params = validParams<AuxKernel>();
    params.addParam<Real>("density", 1000.,"fluid density in Kg/m^3");

    return params;
}

CoupledDensityAux::CoupledDensityAux(const std::string & name,
                                           InputParameters parameters)
:AuxKernel(name, parameters),
_input_density_mixture(getParam<Real>("density"))

{}

Real
CoupledDensityAux::computeValue()
{
    
    return _u[_qp];
    
}

