/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "OutFlowBC_PH.h"

template<>
InputParameters validParams<OutFlowBC_PH>()
{
    InputParameters params = validParams<IntegratedBC>();
    params.addCoupledVar("pressure", "Non-linear pressure variable, [Pa]");
    params.addCoupledVar("temperature", "Non-linear temperature variable, [K]");    
    return params;
}

OutFlowBC_PH::OutFlowBC_PH(const std::string & name, InputParameters parameters)
:IntegratedBC(name, parameters),

_grad_p(coupledGradient("pressure")),
_p_var(coupled("pressure")), 
_grad_T(coupledGradient("temperature")),

_dTdH(getMaterialProperty<Real>("dTdH_P")),
_dTdP(getMaterialProperty<Real>("dTdP_H")),
_enthalpy_water(getMaterialProperty<Real>("enthalpy_water")),
_enthalpy_steam(getMaterialProperty<Real>("enthalpy_steam")),
_denthalpy_waterdH_P(getMaterialProperty<Real>("denthalpy_waterdH_P")),
_denthalpy_steamdH_P(getMaterialProperty<Real>("denthalpy_steamdH_P")),
_denthalpy_waterdP_H(getMaterialProperty<Real>("denthalpy_waterdP_H")),
_denthalpy_steamdP_H(getMaterialProperty<Real>("denthalpy_steamdP_H")),

_Dtau_waterDP(getMaterialProperty<Real>("Dtau_waterDP")),
_Dtau_waterDH(getMaterialProperty<Real>("Dtau_waterDH")),
_Dtau_steamDP(getMaterialProperty<Real>("Dtau_steamDP")),
_Dtau_steamDH(getMaterialProperty<Real>("Dtau_steamDH")),
_tau_water(getMaterialProperty<Real>("tau_water")),
_tau_steam(getMaterialProperty<Real>("tau_steam")),
_darcy_mass_flux_water(getMaterialProperty<RealGradient>("darcy_mass_flux_water")),
_darcy_mass_flux_steam(getMaterialProperty<RealGradient>("darcy_mass_flux_steam")),
_thermal_conductivity(getMaterialProperty<Real>("thermal_conductivity"))
{}

Real
OutFlowBC_PH::computeQpResidual()
{
    //    RealGradient _Darcy_vel = -_cond*_grad_p[_qp];
    
    Real _aa = _test[_i][_qp]*
    ( _thermal_conductivity[_qp] *(_grad_T[_qp]*_normals[_qp])
     - _darcy_mass_flux_water[_qp] * _enthalpy_water[_qp] *_normals[_qp]
     - _darcy_mass_flux_steam[_qp] * _enthalpy_steam[_qp] *_normals[_qp]);
    
    return -_aa;
}


Real
OutFlowBC_PH::computeQpJacobian()
{
  Real _aa=_test[_i][_qp]*
            (_thermal_conductivity[_qp]*_dTdH[_qp]*_grad_phi[_j][_qp]*_normals[_qp]
             +_Dtau_waterDH[_qp]*_phi[_j][_qp]*_grad_p[_qp]*_enthalpy_water[_qp]*_normals[_qp]
             -_darcy_mass_flux_water[_qp]*_denthalpy_waterdH_P[_qp]*_phi[_j][_qp]*_normals[_qp]
             +_Dtau_steamDH[_qp]*_phi[_j][_qp]*_grad_p[_qp]*_enthalpy_steam[_qp]*_normals[_qp]
             -_darcy_mass_flux_steam[_qp]*_denthalpy_steamdH_P[_qp]*_phi[_j][_qp]*_normals[_qp]);    
    
    return -_aa;
}

Real OutFlowBC_PH::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar==_p_var)
  {
    Real _bb=_test[_i][_qp]*
             (_thermal_conductivity[_qp]*_dTdP[_qp]*_grad_phi[_j][_qp]*_normals[_qp]
              +(_Dtau_waterDP[_qp]*_phi[_j][_qp]*_grad_p[_qp]+_tau_water[_qp]*_grad_phi[_j][_qp])*_enthalpy_water[_qp]*_normals[_qp]
              -_darcy_mass_flux_water[_qp]*_denthalpy_waterdP_H[_qp]*_phi[_j][_qp]*_normals[_qp]
              +(_Dtau_steamDP[_qp]*_phi[_j][_qp]*_grad_p[_qp]+_tau_steam[_qp]*_grad_phi[_j][_qp])*_enthalpy_steam[_qp]*_normals[_qp]
              -_darcy_mass_flux_steam[_qp]*_denthalpy_steamdP_H[_qp]*_phi[_j][_qp]*_normals[_qp] );
    
      
    return -_bb;
    
 }
 else
 {
   return 0.0;
 }
  
}

  
