#include "ApertureAux.h"

template<>
InputParameters validParams<ApertureAux>()
{
	InputParameters params = validParams<AuxKernel>();
     return params;
}

ApertureAux::ApertureAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
   _aperture(getMaterialProperty<Real>("aperture"))

{}

Real
ApertureAux::computeValue()
{
  return _aperture[_qp];


}
