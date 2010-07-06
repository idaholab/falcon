#include "CoupledRhoAux.h"

template<>
InputParameters validParams<CoupledRhoAux>()
{
  InputParameters params = validParams<AuxKernel>();
  return params;
}

CoupledRhoAux::CoupledRhoAux(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :AuxKernel(name, moose_system, parameters),
   _rho_w(getMaterialProperty<Real>("rho_w")),
   _rho_s(getMaterialProperty<Real>("rho_s")),
   _sat_w(getMaterialProperty<Real>("sat_w")),
   _sat_s(getMaterialProperty<Real>("sat_s"))
{}


Real
CoupledRhoAux::computeValue()
{
              
 return  (_sat_w[_qp]*_rho_w[_qp])+(_sat_s[_qp]*_rho_s[_qp]);

}
