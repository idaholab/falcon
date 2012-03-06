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

#include "CoupledSteamDensityAux.h"

template<>
InputParameters validParams<CoupledSteamDensityAux>()
{
    InputParameters params = validParams<AuxKernel>();
    params.addParam<Real>("density_steam", 1.,"fluid density in Kg/m^3");
    return params;
}

CoupledSteamDensityAux::CoupledSteamDensityAux(const std::string & name,
                                               InputParameters parameters)
:AuxKernel(name, parameters),
_input_density_steam(getParam<Real>("density_steam"))

{}

Real
CoupledSteamDensityAux::computeValue()
{
        return _u[_qp];                           
}

