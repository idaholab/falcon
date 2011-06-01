#include "CoupledDdensityDTAux.h"

template<>
InputParameters validParams<CoupledDdensityDTAux>()
{
  InputParameters params = validParams<AuxKernel>();
   return params;
}

CoupledDdensityDTAux::CoupledDdensityDTAux(const std::string & name,
                                     InputParameters parameters)
  :AuxKernel(name, parameters)
{}

Real
CoupledDdensityDTAux::computeValue()
{
    return _u[_qp];

}

