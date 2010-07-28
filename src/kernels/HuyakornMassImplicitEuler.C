
#include "HuyakornMassImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<HuyakornMassImplicitEuler>()
{
     InputParameters params = validParams<ImplicitEuler>();
     return params;
}

HuyakornMassImplicitEuler::HuyakornMassImplicitEuler(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :ImplicitEuler(name, moose_system, parameters),

   _rho(getMaterialProperty<Real>("rho")),
   _rho_old(getMaterialProperty<Real>("rho_old")),
   _porosity(getMaterialProperty<Real>("porosity"))
        
{ }

Real
HuyakornMassImplicitEuler::computeQpResidual()
{
  Real cf = 1e-5;
  
  // return _porosity[_qp]*cf*_rho[_qp]*ImplicitEuler::computeQpResidual();
  
  return _porosity[_qp]*_test[_i][_qp]*(_rho[_qp] -_rho_old[_qp])/_dt;
}

Real
HuyakornMassImplicitEuler::computeQpJacobian()
{
    
   return 0;
     
}
