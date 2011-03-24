#include "HuyakornEnthalpyImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<HuyakornEnthalpyImplicitEuler>()
{
  InputParameters params = validParams<ImplicitEuler>();
  return params;
}

HuyakornEnthalpyImplicitEuler::HuyakornEnthalpyImplicitEuler(const std::string & name, InputParameters parameters)
  :ImplicitEuler(name, parameters),
   
   _temp(getMaterialProperty<Real>("temperature")),
   _temp_old(getMaterialProperty<Real>("temperature_old")),
   _density(getMaterialProperty<Real>("density")),
   _density_old(getMaterialProperty<Real>("density_old")),

   _Heat(getMaterialProperty<Real>("Heat")),
   _Heat_old(getMaterialProperty<Real>("Heat_old")),
   
   _porosity(getMaterialProperty<Real>("porosity")),
   _density_rock(getMaterialProperty<Real>("density_rock")),
   _specific_heat_rock(getMaterialProperty<Real>("specific_heat_rock"))
   

{}

Real
HuyakornEnthalpyImplicitEuler::computeQpResidual()
{
     
  Real H = (_porosity[_qp]* _density[_qp]*_u[_qp])+((1-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]*_temp[_qp]);
  Real H_old = (_porosity[_qp]* _density_old[_qp]*_u_old[_qp])+((1-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]*_temp_old[_qp]);
    
  return _test[_i][_qp]*(H-H_old)/_dt;
//  return _test[_i][_qp]*(_Heat[_qp]-_Heat_old[_qp])/_dt;
}

Real
HuyakornEnthalpyImplicitEuler::computeQpJacobian()
{

   return (_porosity[_qp]* _density[_qp]*ImplicitEuler::computeQpJacobian());
  
}
