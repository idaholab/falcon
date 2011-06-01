#include "TemperatureTimeDerivative.h"
#include "Material.h"

template<>
InputParameters validParams<TemperatureTimeDerivative>()
{
  InputParameters params = validParams<TimeDerivative>();
  params.addRequiredCoupledVar("density_water", "Use CoupledAuxDensity here");
  params.addRequiredCoupledVar("dwdt", "derivative of water density vs temperature");
  return params;
}

TemperatureTimeDerivative::TemperatureTimeDerivative(const std::string & name,
                                                     InputParameters parameters)
  :TimeDerivative(name, parameters),
   _density_water(coupledValue("density_water")),
   _density_water_old(coupledValueOld("density_water")),
   _dwdt(coupledValue("dwdt")),
   _porosity (getMaterialProperty<Real>("material_porosity")),
   _specific_heat_water(getMaterialProperty<Real>("specific_heat_water")),
   _specific_heat_rock(getMaterialProperty<Real>("specific_heat_rock")),
   _density_rock(getMaterialProperty<Real>("density_rock"))
{}

Real
TemperatureTimeDerivative::computeQpResidual()
{
  Real tmp1=(((_porosity[_qp]*_density_water[_qp]*_specific_heat_water[_qp])+
              ((1.0-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]))*_u[_qp]-
              ((_porosity[_qp]*_density_water_old[_qp]*_specific_heat_water[_qp])+
               ((1.0-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]))
                *_u_old[_qp])*_test[_i][_qp]/_dt;
  return tmp1;
}

Real
TemperatureTimeDerivative::computeQpJacobian()
{
/*    
  Real tmp1 = (((_porosity[_qp]*_density_water[_qp]*_specific_heat_water[_qp])+
       ((1.0-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]))*_phi[_j][_qp])*_test[_i][_qp]/_dt;
  Real tmp2 = _porosity[_qp]*_dwdt[_qp]*_specific_heat_water[_qp]*_u[_qp]*_test[_i][_qp]/_dt;
*/
  Real tmp1 = (((_porosity[_qp]*_density_water[_qp]*_specific_heat_water[_qp])+
               ((1.0-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]))*
               _phi[_j][_qp])*_test[_i][_qp]/_dt;  
  return tmp1;
}
