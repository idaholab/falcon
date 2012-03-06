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

#include "CoupledWaterDensityAux.h"

template<>
InputParameters validParams<CoupledWaterDensityAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addParam<Real>("density_water", 1000.,"fluid density in Kg/m^3");
  return params;
}

CoupledWaterDensityAux::CoupledWaterDensityAux(const std::string & name,
                                     InputParameters parameters)
  :AuxKernel(name, parameters),
   _input_density_water(getParam<Real>("density_water"))
   
{}

Real
CoupledWaterDensityAux::computeValue()
{
        return _u[_qp];                           
}

