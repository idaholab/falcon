#include "MassFluxTimeDerivative.h"
#include "Material.h"

template<>
InputParameters validParams<MassFluxTimeDerivative>()
{
  InputParameters params = validParams<TimeDerivative>();
   params.addRequiredCoupledVar("density_water", "Use Coupled density here to calculate the time derivative");
   return params;
}

MassFluxTimeDerivative::MassFluxTimeDerivative(const std::string & name, InputParameters parameters)
  :TimeDerivative(name, parameters),
   
   _density_water(coupledValue("density_water")),
   _density_water_old(coupledValueOld("density_water")),
   _porosity (getMaterialProperty<Real>("material_porosity"))
//   _porosity(coupledValue("porosity")),
//   _porosity_old(coupledValueOld("porosity"))

{}

Real
MassFluxTimeDerivative::computeQpResidual()
{
//  std::cout << ((_porosity[_qp]*_density_water[_qp])-(_porosity_old[_qp]*_density_water_old[_qp]))/_dt << "\n";
  return (((_porosity[_qp]*_density_water[_qp])-(_porosity[_qp]*_density_water_old[_qp]))/_dt) * _test[_i][_qp];
}

Real
MassFluxTimeDerivative::computeQpJacobian()
{    
return 0;
}
