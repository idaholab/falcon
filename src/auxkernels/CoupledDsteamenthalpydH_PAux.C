#include "CoupledDsteamenthalpydH_PAux.h"

template<>
InputParameters validParams<CoupledDsteamenthalpydH_PAux>()
{
    InputParameters params = validParams<AuxKernel>();
    return params;
}

CoupledDsteamenthalpydH_PAux::CoupledDsteamenthalpydH_PAux(const std::string & name,
                                                           InputParameters parameters)
:AuxKernel(name, parameters)

{}

Real
CoupledDsteamenthalpydH_PAux::computeValue()
{
    return _u[_qp];                           
}

