#include "HuyakornEnthalpyImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<HuyakornEnthalpyImplicitEuler>()
{
  InputParameters params = validParams<ImplicitEuler>();
  return params;
}

HuyakornEnthalpyImplicitEuler::HuyakornEnthalpyImplicitEuler(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :ImplicitEuler(name, moose_system, parameters),
   
   _temp(getMaterialProperty<Real>("temperature")),
   _temp_old(getMaterialProperty<Real>("temperature_old")),
   _rho(getMaterialProperty<Real>("rho")),
   _rho_old(getMaterialProperty<Real>("rho_old")),

   _Heat(getMaterialProperty<Real>("Heat")),
   _Heat_old(getMaterialProperty<Real>("Heat_old")),
   
   _porosity(getMaterialProperty<Real>("porosity")),
   _rho_r(getMaterialProperty<Real>("rho_r")),
   _cp_r(getMaterialProperty<Real>("cp_r"))
   

{}

Real
HuyakornEnthalpyImplicitEuler::computeQpResidual()
{
     
  Real H = (_porosity[_qp]* _rho[_qp]*_u[_qp])+((1-_porosity[_qp])*_rho_r[_qp]*_cp_r[_qp]*_temp[_qp]);
  Real H_old = (_porosity[_qp]* _rho_old[_qp]*_u_old[_qp])+((1-_porosity[_qp])*_rho_r[_qp]*_cp_r[_qp]*_temp_old[_qp]);
    
  return _test[_i][_qp]*(H-H_old)/_dt;
//  return _test[_i][_qp]*(_Heat[_qp]-_Heat_old[_qp])/_dt;
}

Real
HuyakornEnthalpyImplicitEuler::computeQpJacobian()
{

   return (_porosity[_qp]* _rho[_qp]*ImplicitEuler::computeQpJacobian());
  
}
