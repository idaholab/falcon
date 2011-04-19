#include "TemperatureTimeDerivative.h"
#include "Material.h"

template<>
InputParameters validParams<TemperatureTimeDerivative>()
{
  InputParameters params = validParams<TimeDerivative>();
  params.addRequiredCoupledVar("density_water", "Use CoupledAuxDensity here");
  params.addRequiredCoupledVar("porosity", "Use CoupledAuxPorosity here");
  return params;
}

TemperatureTimeDerivative::TemperatureTimeDerivative(const std::string & name,
                                                     InputParameters parameters)
  :TimeDerivative(name, parameters),


   _density_water(coupledValue("density_water")),
   _density_water_old(coupledValueOld("density_water")),
   
   _porosity(coupledValue("porosity")),
   _porosity_old(coupledValueOld("porosity")),

   _specific_heat_water(getMaterialProperty<Real>("specific_heat_water")),
   _specific_heat_rock(getMaterialProperty<Real>("specific_heat_rock")),
   _density_rock(getMaterialProperty<Real>("density_rock"))
   
{}

Real
TemperatureTimeDerivative::computeQpResidual()
{
  
  //REAL dphirho_dt = ((_porosity[_qp]*_density_water[_qp])-(_porosity_old[_qp]*_density_water_old[_qp]))/_dt;
  
  return
    ((_porosity[_qp]*_density_water[_qp]*_specific_heat_water[_qp])

     +

     ((1.0-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]))

    * TimeDerivative::computeQpResidual();
}

Real
TemperatureTimeDerivative::computeQpJacobian()
{
  return
    ((_porosity[_qp]*_density_water[_qp]*_specific_heat_water[_qp])
     
     +
     
     ((1.0-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]))
    
    * TimeDerivative::computeQpJacobian();
  
}
