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

#include "CoupledDTDH_PAux.h"

template<>
InputParameters validParams<CoupledDTDH_PAux>()
{
    InputParameters params = validParams<AuxKernel>();
  
    return params;
}

CoupledDTDH_PAux::CoupledDTDH_PAux(const std::string & name,
                                           InputParameters parameters)
:AuxKernel(name, parameters)
{}

Real
CoupledDTDH_PAux::computeValue()
{
    
    return _u[_qp];
    
}
