#include "EnthalpyImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<EnthalpyImplicitEuler>()
{
  InputParameters params = validParams<ImplicitEuler>();
  params.addCoupledVar("tempAux", "TODO: add description");
  return params;
}

EnthalpyImplicitEuler::EnthalpyImplicitEuler(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :ImplicitEuler(name, moose_system, parameters),
   _temperature(coupledVal("tempAux")),
   _temperature_old(coupledValOld("tempAux")),
   _porosity(getRealMaterialProperty("porosity")),
//  _temperature(getRealMaterialProperty("tempMat")),
//   _temperature_old(getRealMaterialPropertyOld("tempMat")),
   _rho_mix(getRealMaterialProperty("rho_mix")),
   _rho_r(getRealMaterialProperty("rho_r"))
   
{}

Real
EnthalpyImplicitEuler::computeQpResidual()
{

  
  //std:: cout << "temperature"<< _temperature[_qp];
  //  std:: cout << "temperature_old"<< _temperature_old[_qp];

   Real Resi;
   Resi = (_porosity[_qp]* _rho_mix[_qp]*ImplicitEuler::computeQpResidual())+((1-_porosity[_qp])*_rho_r[_qp]*879*(_temperature[_qp]-_temperature_old[_qp])/_dt);

   // std::cout << "ResiEnth1" << Resi << " .\n";
  
return (_porosity[_qp]* _rho_mix[_qp]*ImplicitEuler::computeQpResidual())+((1-_porosity[_qp])*_rho_r[_qp]*879*(_temperature[_qp]-_temperature_old[_qp])/_dt);



}

Real
EnthalpyImplicitEuler::computeQpJacobian()
{

  
  return (_porosity[_qp]* _rho_mix[_qp]*ImplicitEuler::computeQpJacobian());
  
}
