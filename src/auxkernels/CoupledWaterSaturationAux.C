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

#include "CoupledWaterSaturationAux.h"

template<>
InputParameters validParams<CoupledWaterSaturationAux>()
{
    InputParameters params = validParams<AuxKernel>();
    params.addParam<Real>("saturation_water", 1.,"fluid density in Kg/m^3");
    return params;
}

CoupledWaterSaturationAux::CoupledWaterSaturationAux(const std::string & name,
                                               InputParameters parameters)
:AuxKernel(name, parameters),
_input_saturation_water(getParam<Real>("saturation_water"))
{}

Real
CoupledWaterSaturationAux::computeValue()
{
        return _u[_qp];                           
}

