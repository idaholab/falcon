#include "PermeabilityAux.h"

template<>
InputParameters validParams<PermeabilityAux>()
{
	InputParameters params = validParams<AuxKernel>();
     return params;
}

PermeabilityAux::PermeabilityAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
   _permeability(getMaterialProperty<Real>("permeability"))

{}

Real
PermeabilityAux::computeValue()
{
  return _permeability[_qp];


}
