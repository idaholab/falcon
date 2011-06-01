#include "CoupledDdensityDHAux.h"

template<>
InputParameters validParams<CoupledDdensityDHAux>()
{
    InputParameters params = validParams<AuxKernel>();
    return params;
}

CoupledDdensityDHAux::CoupledDdensityDHAux(const std::string & name,
                                           InputParameters parameters)
:AuxKernel(name, parameters)
{}

Real
CoupledDdensityDHAux::computeValue()
{
    
    return _u[_qp];
    
}

