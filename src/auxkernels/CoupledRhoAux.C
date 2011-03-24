#include "CoupledRhoAux.h"

template<>
InputParameters validParams<CoupledRhoAux>()
{
  InputParameters params = validParams<AuxKernel>();
  return params;
}

CoupledRhoAux::CoupledRhoAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
   _density_water(getMaterialProperty<Real>("density_water")),
   _density_steam(getMaterialProperty<Real>("density_steam")),
   _S_water(getMaterialProperty<Real>("S_water")),
   _S_steam(getMaterialProperty<Real>("S_steam"))
{}


Real
CoupledRhoAux::computeValue()
{
              
 return  (_S_water[_qp]*_density_water[_qp])+(_S_steam[_qp]*_density_steam[_qp]);

}
