
#include "HuyakornMassImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<HuyakornMassImplicitEuler>()
{
     InputParameters params = validParams<ImplicitEuler>();
     return params;
}

HuyakornMassImplicitEuler::HuyakornMassImplicitEuler(const std::string & name, InputParameters parameters)
  :ImplicitEuler(name, parameters),

   _density(getMaterialProperty<Real>("density")),
   _density_old(getMaterialProperty<Real>("density_old")),
   _porosity(getMaterialProperty<Real>("porosity"))
        
{ }

Real
HuyakornMassImplicitEuler::computeQpResidual()
{
  Real cf = 1e-5;
  
  // return _porosity[_qp]*cf*_density[_qp]*ImplicitEuler::computeQpResidual();
  
   return _porosity[_qp]*_test[_i][_qp]*(_density[_qp] -_density_old[_qp])/_dt;
}

Real
HuyakornMassImplicitEuler::computeQpJacobian()
{
    
   return 0;
     
}
