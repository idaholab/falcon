#include "EnthalpyImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<EnthalpyImplicitEuler>()
{
  InputParameters params = validParams<ImplicitEuler>();
  params.addCoupledVar("tempAux", "TODO: add description");
  params.addCoupledVar("rhoAux", "TODO: add description");
  return params;
}

EnthalpyImplicitEuler::EnthalpyImplicitEuler(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :ImplicitEuler(name, moose_system, parameters),
   _temperature(coupledValue("tempAux")),
   _temperature_old(coupledValueOld("tempAux")),
   _porosity(getMaterialProperty<Real>("porosity")),
//  _temperature(getMaterialProperty<Real>("tempMat")),
//   _temperature_old(getMaterialPropertyOld<Real>("tempMat")),
   _rho(getMaterialProperty<Real>("rho")),
   _rho_r(getMaterialProperty<Real>("rho_r"))
   

{}

Real
EnthalpyImplicitEuler::computeQpResidual()
{

  return (_porosity[_qp]* _rho[_qp]*ImplicitEuler::computeQpResidual())+((1-_porosity[_qp])*_rho_r[_qp]*879*(_temperature[_qp]-_temperature_old[_qp])/_dt);

}

Real
EnthalpyImplicitEuler::computeQpJacobian()
{

   return (_porosity[_qp]* _rho[_qp]*ImplicitEuler::computeQpJacobian());
  
}
