#include "CoupledDTDH_PAux.h"

template<>
InputParameters validParams<CoupledDTDH_PAux>()
{
    InputParameters params = validParams<AuxKernel>();
  
    return params;
}

CoupledDTDH_PAux::CoupledDTDH_PAux(const std::string & name,
                                           InputParameters parameters)
:AuxKernel(name, parameters)
{}

Real
CoupledDTDH_PAux::computeValue()
{
    
    return _u[_qp];
    
}
