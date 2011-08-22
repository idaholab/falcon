#include "CoupledDsteamenthalpydP_HAux.h"

template<>
InputParameters validParams<CoupledDsteamenthalpydP_HAux>()
{
    InputParameters params = validParams<AuxKernel>();
    return params;
}

CoupledDsteamenthalpydP_HAux::CoupledDsteamenthalpydP_HAux(const std::string & name,
                                                           InputParameters parameters)
:AuxKernel(name, parameters)

{}

Real
CoupledDsteamenthalpydP_HAux::computeValue()
{
    return _u[_qp];                           
}

