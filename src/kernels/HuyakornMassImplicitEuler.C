
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
  if ((_u[_qp] < 10000) || (_u[_qp] >20000000))
       {
         std::cout<<"pressure "<<_u[_qp]<<".\n";
         std::cout<<"_rho[_qp] "<<_rho[_qp]<<".\n";
         std::cout<<"_rho_old[_qp] "<<_rho_old[_qp]<<".\n";
         std::cout<<"porosity "<<_porosity[_qp]<<".\n";
         mooseError("pressure out of Range");
       }
  
     return _porosity[_qp]*_test[_i][_qp]*(_rho[_qp] -_rho_old[_qp])/_dt;
}

Real
HuyakornMassImplicitEuler::computeQpJacobian()
{
    
   return 0;
     
}
