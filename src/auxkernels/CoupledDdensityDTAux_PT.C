#include "CoupledDdensityDTAux_PT.h"
#include "Water_Steam_EOS.h"

template<>
InputParameters validParams<CoupledDdensityDTAux_PT>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addCoupledVar("temperature", "Use temperature to calculate variable density");
  params.addCoupledVar("pressure", "Use pressure to calculate variable density");
  return params;
}

CoupledDdensityDTAux_PT::CoupledDdensityDTAux_PT(const std::string & name,
                                     InputParameters parameters)
  :AuxKernel(name, parameters),
   _temperature(coupledValue("temperature")),
   _pressure(coupledValue("pressure"))
{}

Real
CoupledDdensityDTAux_PT::computeValue()
{
  return _u[_qp]; //dummy aux kernel, value updated in CoupledDensityAux_PT kenerl
}
