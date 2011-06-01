#include "OutFlowBC.h"

template<>
InputParameters validParams<OutFlowBC>()
{
  InputParameters params = validParams<IntegratedBC>();
 // params.set<Real>("thermal_conductivity") = 5.0;
//  params.set<Real>("conductivity")= 0.0;
//  params.set<Real>("porosity")    = 1.0;
   params.addCoupledVar("pressure", "Use pressure here");
   params.addCoupledVar("temperature", "Use pressure here"); 
   params.addCoupledVar("enthalpy_water", "Use pressure here");
   params.addCoupledVar("enthalpy_steam", "Use pressure here");
   params.addCoupledVar("dTdH_P", "Use pressure here");
   params.addCoupledVar("denthalpy_waterdH_P", "Use pressure here");
   params.addCoupledVar("denthalpy_steamdH_P", "Use pressure here");
   return params;
}

OutFlowBC::OutFlowBC(const std::string & name, InputParameters parameters)
  :IntegratedBC(name, parameters),
   _grad_p(coupledGradient("pressure")),
   _grad_T(coupledGradient("temperature")),
//   _cond(getParam<Real>("conductivity")),
   _enthalpy_water(coupledValue("enthalpy_water")),
   _enthalpy_steam(coupledValue("enthalpy_steam")),
 
   _dTdH(coupledValue("dTdH_P")),
   _denthalpy_waterdH(coupledValue("denthalpy_waterdH_P")),
   _denthalpy_steamdH(coupledValue("denthalpy_steamdH_P")),

   
   _darcy_mass_flux_water(getMaterialProperty<RealGradient>("darcy_mass_flux_water")),
   
   _darcy_mass_flux_steam(getMaterialProperty<RealGradient>("darcy_mass_flux_steam")),

   _thermal_conductivity(getMaterialProperty<Real>("thermal_conductivity"))

{}

Real
OutFlowBC::computeQpResidual()
  {
//    RealGradient _Darcy_vel = -_cond*_grad_p[_qp];
//    std::cout<<"Darcy velocity" << _Darcy_vel(1);
//    std::cout << "porosity,diffusivity, cond " << _porosity <<" " << _diff <<" "<< _cond << std::endl;

    Real _aa = _test[_i][_qp]*
      ( -_thermal_conductivity[_qp] *_grad_T[_qp]*_normals[_qp] 
       + _darcy_mass_flux_water[_qp]* _enthalpy_water[_qp]*_normals[_qp]
       + _darcy_mass_flux_steam[_qp]* _enthalpy_steam[_qp]*_normals[_qp]);
      
    return _aa;
  }


Real
OutFlowBC::computeQpJacobian()
  {
//    RealGradient _Darcy_vel = -_cond*_grad_p[_qp];
      Real _aa=  _test[_i][_qp]*
      ( -_thermal_conductivity[_qp] *(_dTdH[_qp]*_grad_phi[_j][_qp]*_normals[_qp])
       + _darcy_mass_flux_water[_qp] *_denthalpy_waterdH[_qp] *_phi[_j][_qp]* _normals[_qp]
       + _darcy_mass_flux_steam[_qp] *_denthalpy_steamdH[_qp] *_phi[_j][_qp]* _normals[_qp]);
      
      return _aa;
  
  }
