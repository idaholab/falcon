#include "CoupledSteamEnthalpyAux.h"

template<>
InputParameters validParams<CoupledSteamEnthalpyAux>()
{
    InputParameters params = validParams<AuxKernel>();
    params.addParam<Real>("enthalpy_steam", 278.,"fluid density in Kg/m^3");
    return params;
}

CoupledSteamEnthalpyAux::CoupledSteamEnthalpyAux(const std::string & name,
                                                 InputParameters parameters)
:AuxKernel(name, parameters),
_input_enthalpy_steam(getParam<Real>("enthalpy_steam"))

{}

Real
CoupledSteamEnthalpyAux::computeValue()
{
    return _u[_qp];                           
}
