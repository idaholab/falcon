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

