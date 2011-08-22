#include "CoupledDTDP_HAux.h"

template<>
InputParameters validParams<CoupledDTDP_HAux>()
{
    InputParameters params = validParams<AuxKernel>();
  
    return params;
}

CoupledDTDP_HAux::CoupledDTDP_HAux(const std::string & name,
                                           InputParameters parameters)
:AuxKernel(name, parameters)
{}

Real
CoupledDTDP_HAux::computeValue()
{
    
    return _u[_qp];
    
}
