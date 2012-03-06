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

