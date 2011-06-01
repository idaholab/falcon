#include "CoupledDwaterenthalpydH_PAux.h"

template<>
InputParameters validParams<CoupledDwaterenthalpydH_PAux>()
{
    InputParameters params = validParams<AuxKernel>();
      return params;
}

CoupledDwaterenthalpydH_PAux::CoupledDwaterenthalpydH_PAux(const std::string & name,
                                               InputParameters parameters)
:AuxKernel(name, parameters)

{}

Real
CoupledDwaterenthalpydH_PAux::computeValue()
{
    return _u[_qp];                           
}

