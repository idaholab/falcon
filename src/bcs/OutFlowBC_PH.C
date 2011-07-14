#include "OutFlowBC_PH.h"

template<>
InputParameters validParams<OutFlowBC_PH>()
{
    InputParameters params = validParams<IntegratedBC>();
    params.addCoupledVar("pressure", "Use pressure here");
    params.addCoupledVar("temperature", "Use temperature here");
    params.addCoupledVar("dTdH_P", "Use dTdH_P here");
    params.addCoupledVar("enthalpy_water", "Use enthalpy_water here");
    params.addCoupledVar("enthalpy_steam", "Use enthalpy_steam here");
    params.addCoupledVar("denthalpy_waterdH_P", "Use denthalpy_waterdH_P here");
    params.addCoupledVar("denthalpy_steamdH_P", "Use denthalpy_steamdH_P here");
    //  params.set<Real>("conductivity")= 0.0;
    return params;
}

OutFlowBC_PH::OutFlowBC_PH(const std::string & name, InputParameters parameters)
:IntegratedBC(name, parameters),
//   _grad_p(coupledGradient("p")),
//   _cond(getParam<Real>("conductivity")),
//   _diff(getParam<Real>("thermal_conductivity")),
//   _porosity(getParam<Real>("porosity"))
_grad_p(coupledGradient("pressure")),
_grad_T(coupledGradient("temperature")),
_dTdH(coupledValue("dTdH_P")),
_enthalpy_water(coupledValue("enthalpy_water")),
_enthalpy_steam(coupledValue("enthalpy_steam")),
_denthalpy_waterdH_P(coupledValue("denthalpy_waterdH_P")),
_denthalpy_steamdH_P(coupledValue("denthalpy_steamdH_P")),
_darcy_mass_flux_water(getMaterialProperty<RealGradient>("darcy_mass_flux_water")),
_darcy_mass_flux_steam(getMaterialProperty<RealGradient>("darcy_mass_flux_steam")),
_Ddarcy_mass_flux_waterDH(getMaterialProperty<RealGradient>("Ddarcy_mass_flux_waterDH")),
_Ddarcy_mass_flux_steamDH(getMaterialProperty<RealGradient>("Ddarcy_mass_flux_steamDH")),

_thermal_conductivity(getMaterialProperty<Real>("thermal_conductivity"))
{}

Real
OutFlowBC_PH::computeQpResidual()
{
    //    RealGradient _Darcy_vel = -_cond*_grad_p[_qp];
    //    std::cout<<"Darcy velocity" << _Darcy_vel(1);
    //    std::cout << "porosity,diffusivity, cond " << _porosity <<" " << _diff <<" "<< _cond << std::endl;
    
    Real _aa = _test[_i][_qp]*
    ( _thermal_conductivity[_qp] *(_grad_T[_qp]*_normals[_qp])
     - _darcy_mass_flux_water[_qp] * _enthalpy_water[_qp] *_normals[_qp]
     - _darcy_mass_flux_steam[_qp] * _enthalpy_steam[_qp] *_normals[_qp]);
    
    //    if (_aa <= 1.0e-12)
    //      _aa=0.0;
    
    //    std::cout << "utlet_flux " << _aa << std::endl;
    
    return -_aa;
}


Real
OutFlowBC_PH::computeQpJacobian()
{
    //    RealGradient _Darcy_vel = -_cond*_grad_p[_qp];
    Real _aa=  _test[_i][_qp]*
    ( _thermal_conductivity[_qp] *(_dTdH[_qp]*_grad_phi[_j][_qp]*_normals[_qp])
     - _darcy_mass_flux_water[_qp] * _denthalpy_waterdH_P[_qp] * _phi[_j][_qp]* _normals[_qp]
     - _Ddarcy_mass_flux_waterDH[_qp] * _enthalpy_water[_qp] * _phi[_j][_qp] * _normals[_qp]
     - _darcy_mass_flux_steam[_qp] * _denthalpy_steamdH_P[_qp] * _phi[_j][_qp]* _normals[_qp]
     - _Ddarcy_mass_flux_steamDH[_qp] * _enthalpy_steam[_qp] * _phi[_j][_qp]* _normals[_qp]);
    
    return -_aa;
}
