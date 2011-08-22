#include "CoupledDwaterenthalpydP_HAux.h"

template<>
InputParameters validParams<CoupledDwaterenthalpydP_HAux>()
{
    InputParameters params = validParams<AuxKernel>();
    return params;
}

CoupledDwaterenthalpydP_HAux::CoupledDwaterenthalpydP_HAux(const std::string & name,
                                                           InputParameters parameters)
:AuxKernel(name, parameters)

{}

Real
CoupledDwaterenthalpydP_HAux::computeValue()
{
    return _u[_qp];                           
}

