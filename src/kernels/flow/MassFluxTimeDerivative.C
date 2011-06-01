#include "MassFluxTimeDerivative.h"
#include "Material.h"

template<>
InputParameters validParams<MassFluxTimeDerivative>()
{
  InputParameters params = validParams<TimeDerivative>();
   params.addRequiredCoupledVar("density", "Use Coupled density here to calculate the time derivative");
   params.addRequiredCoupledVar("ddensitydp_H", "derivative of water density vs temperature");
    return params;
}

MassFluxTimeDerivative::MassFluxTimeDerivative(const std::string & name, InputParameters parameters)
  :TimeDerivative(name, parameters),
   
   _density(coupledValue("density")),
   _density_old(coupledValueOld("density")),
   _ddensitydp_H(coupledValue("ddensitydp_H")),

   _porosity (getMaterialProperty<Real>("material_porosity"))
//   _porosity(coupledValue("porosity")),
//   _porosity_old(coupledValueOld("porosity"))

{}

Real
MassFluxTimeDerivative::computeQpResidual()
{
  //std::cout << _density[_qp] << " " << _density_old[_qp] << "\n";
  return (((_porosity[_qp]*_density[_qp])-(_porosity[_qp]*_density_old[_qp]))/_dt) * _test[_i][_qp];
}

Real
MassFluxTimeDerivative::computeQpJacobian()
{ 
   // std::cout << _ddensitydp_H[_qp]<<"\n" ;
   Real tmp1 = (_porosity[_qp]*_ddensitydp_H[_qp]*_phi[_j][_qp])*_test[_i][_qp]/_dt;  
    return tmp1;
}
