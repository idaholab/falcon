#include "EnthalpyTimeDerivative.h"
#include "Material.h"

template<>
InputParameters validParams<EnthalpyTimeDerivative>()
{
    InputParameters params = validParams<TimeDerivative>();
    params.addRequiredCoupledVar("density", "Use CoupledAuxDensity here");
    params.addRequiredCoupledVar("temperature", "Use CoupledAuxDensity here");
    params.addRequiredCoupledVar("dTdH_P", "derivative of water density vs temperature");
    params.addRequiredCoupledVar("ddensitydH_P", "derivative of water density vs temperature");
    //  params.addRequiredCoupledVar("porosity", "Use CoupledAuxPorosity here");
    return params;
}

EnthalpyTimeDerivative::EnthalpyTimeDerivative(const std::string & name,
                                                     InputParameters parameters)
:TimeDerivative(name, parameters),

_density(coupledValue("density")),
_density_old(coupledValueOld("density")),
_temperature(coupledValue("temperature")),
_temperature_old(coupledValueOld("temperature")),

_dTdH_P(coupledValue("dTdH_P")),
_ddensitydH_P(coupledValue("ddensitydH_P")),
//   _porosity(coupledValue("porosity")),
//   _porosity_old(coupledValueOld("porosity")),
_porosity (getMaterialProperty<Real>("material_porosity")),
_specific_heat_rock(getMaterialProperty<Real>("specific_heat_rock")),
_density_rock(getMaterialProperty<Real>("density_rock"))

{}

Real
EnthalpyTimeDerivative::computeQpResidual()
{
    
    //REAL dphirho_dt = ((_porosity[_qp]*_density_water[_qp])-(_porosity_old[_qp]*_density_water_old[_qp]))/_dt;
    // std::cout <<_porosity[_qp]<< "\n";
    /*
     Real tmp1=(((_porosity[_qp]*_density_water[_qp]*_specific_heat_water[_qp])+
     ((1.0-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]))*_u[_qp]-
     ((_porosity[_qp]*_density_water_old[_qp]*_specific_heat_water[_qp])+
     ((1.0-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]))*_u_old[_qp])*_test[_i][_qp]/_dt;
     */
    //   std::cout <<"dwdt: "<< _dwdt[_qp]<<' '<<_density_water[_qp]<< "\n";
    Real tmp1=(_porosity[_qp]*_density[_qp]*_u[_qp]+
              (1.0-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]*_temperature[_qp]-
              _porosity[_qp]*_density_old[_qp]*_u_old[_qp]-
                (1.0-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]*_temperature_old[_qp])
              *_test[_i][_qp] /_dt;
    
    //   Std::cout <<_density_water[_qp] << ' '<< _density_water_old[_qp]<< "\n";
    return tmp1;
    
}

Real
EnthalpyTimeDerivative::computeQpJacobian()
{
    /*    
     Real tmp1 = (((_porosity[_qp]*_density_water[_qp]*_specific_heat_water[_qp])+
     ((1.0-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]))*_phi[_j][_qp])*_test[_i][_qp]/_dt;
     Real tmp2 = _porosity[_qp]*_dwdt[_qp]*_specific_heat_water[_qp]*_u[_qp]*_test[_i][_qp]/_dt;
     */
    
    Real tmp1 = (_porosity[_qp]*(_density[_qp]*_phi[_j][_qp]+_ddensitydH_P[_qp]*_u[_qp])+
                (1.0-_porosity[_qp])*_density_rock[_qp]*_specific_heat_rock[_qp]*_dTdH_P[_qp]*_phi[_j][_qp])
                *_test[_i][_qp] /_dt;  
    
    //  std::cout <<tmp1 << ' '<< tmp2 << "\n";    
    
    return tmp1;
    
    
}
