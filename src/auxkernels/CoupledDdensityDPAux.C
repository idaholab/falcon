#include "CoupledDdensityDPAux.h"

template<>
InputParameters validParams<CoupledDdensityDPAux>()
{
    InputParameters params = validParams<AuxKernel>();
    return params;
}

CoupledDdensityDPAux::CoupledDdensityDPAux(const std::string & name,
                                           InputParameters parameters)
 :AuxKernel(name, parameters)
{}

Real
CoupledDdensityDPAux::computeValue()
{
    return _u[_qp];
    
    
}


