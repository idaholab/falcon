#include "CoupledDWaterSaturationDHAux.h"

template<>
InputParameters validParams<CoupledDWaterSaturationDHAux>()
{
    InputParameters params = validParams<AuxKernel>();
    return params;
}

CoupledDWaterSaturationDHAux::CoupledDWaterSaturationDHAux(const std::string & name,
                                                     InputParameters parameters)
:AuxKernel(name, parameters)

{}

Real
CoupledDWaterSaturationDHAux::computeValue()
{
    return _u[_qp];                           
}

