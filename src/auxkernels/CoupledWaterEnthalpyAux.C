#include "CoupledWaterEnthalpyAux.h"

template<>
InputParameters validParams<CoupledWaterEnthalpyAux>()
{
    InputParameters params = validParams<AuxKernel>();
    params.addParam<Real>("enthalpy_water", 60.,"fluid density in Kg/m^3");
    return params;
}

CoupledWaterEnthalpyAux::CoupledWaterEnthalpyAux(const std::string & name,
                                               InputParameters parameters)
:AuxKernel(name, parameters),
_input_enthalpy_water(getParam<Real>("enthalpy_water"))

{}

Real
CoupledWaterEnthalpyAux::computeValue()
{
    return _u[_qp];                           
}
