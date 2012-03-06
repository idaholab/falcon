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

#include "CoupledDdensityDPAux_PT.h"
#include "Water_Steam_EOS.h"

template<>
InputParameters validParams<CoupledDdensityDPAux_PT>()
{
    InputParameters params = validParams<AuxKernel>();
    params.addCoupledVar("temperature", "Use temperature to calculate variable density");
    params.addCoupledVar("pressure", "Use pressure to calculate variable density");
    return params;
}

CoupledDdensityDPAux_PT::CoupledDdensityDPAux_PT(const std::string & name,
                                           InputParameters parameters)
 :AuxKernel(name, parameters),
  _temperature(coupledValue("temperature")),
  _pressure(coupledValue("pressure"))

{}

Real
CoupledDdensityDPAux_PT::computeValue()
{
  return _u[_qp];  // doing nothing aux kernel. value updated in CoupledDensityAux_PT aux kernel
}

