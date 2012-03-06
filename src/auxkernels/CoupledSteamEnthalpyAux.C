/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

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
