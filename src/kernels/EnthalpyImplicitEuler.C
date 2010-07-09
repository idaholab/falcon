#include "EnthalpyImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<EnthalpyImplicitEuler>()
{
  InputParameters params = validParams<ImplicitEuler>();
//  params.addCoupledVar("tempAux", "TODO: add description");
//  params.addCoupledVar("rhoAux", "TODO: add description");
  return params;
}

EnthalpyImplicitEuler::EnthalpyImplicitEuler(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :ImplicitEuler(name, moose_system, parameters),
//   _temperature(coupledValue("tempAux")),
//   _temperature_old(coupledValueOld("tempAux")),
//   _rho(coupledValue("rhoAux")),
//   _rho_old(coupledValueOld("rhoAux")),
   
   _temperature(getMaterialProperty<Real>("temperature")),
   _temperature_old(getMaterialProperty<Real>("temperature_old")),
   _rho(getMaterialProperty<Real>("rho")),
   _rho_old(getMaterialProperty<Real>("rho_old")),

   _porosity(getMaterialProperty<Real>("porosity")),
   _rho_r(getMaterialProperty<Real>("rho_r"))
   

{}

Real
EnthalpyImplicitEuler::computeQpResidual()
{

  
  Real Heat = (_porosity[_qp]* _rho[_qp]*_u[_qp])+((1-_porosity[_qp])*_rho_r[_qp]*879*_temperature[_qp]);
  Real Heat_old = (_porosity[_qp]* _rho_old[_qp]*_u_old[_qp])+((1-_porosity[_qp])*_rho_r[_qp]*879*_temperature_old[_qp]);
    
   return _test[_i][_qp]*(Heat-Heat_old)/_dt;
}

Real
EnthalpyImplicitEuler::computeQpJacobian()
{

   return (_porosity[_qp]* _rho[_qp]*ImplicitEuler::computeQpJacobian());
  
}
