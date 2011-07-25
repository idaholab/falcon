#include "Material.h"
#include "EnthalpyConvectionSteam.h"

template<>
InputParameters validParams<EnthalpyConvectionSteam>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("enthalpy_steam", "Use CoupledAuxPorosity here");
  params.addRequiredCoupledVar("denthalpy_steamdH_P", "Use CoupledAux dsteamenthalpydh_P here");

  params.addCoupledVar("pressure","Use CoupledVariable pressure index here");
  
  return params;
}

EnthalpyConvectionSteam::EnthalpyConvectionSteam(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),

   _darcy_mass_flux_steam(getMaterialProperty<RealGradient>("darcy_mass_flux_steam")),
   _Ddarcy_mass_flux_steamDH(getMaterialProperty<RealGradient>("Ddarcy_mass_flux_steamDH")),
   _tau_steam(getMaterialProperty<Real>("tau_steam")),
   _enthalpy_steam(coupledValue("enthalpy_steam")),
   _denthalpy_steamdH_P(coupledValue("denthalpy_steamdH_P")),
   _p_var(coupled("pressure")),
   _p(coupledValue("pressure"))   
{}

Real EnthalpyConvectionSteam::computeQpResidual()
{


  //return  _darcy_mass_flux_steam[_qp]*_grad_enthalpy_steam[_qp]*_test[_i][_qp];
  return -_darcy_mass_flux_steam[_qp]*_enthalpy_steam[_qp]*_grad_test[_i][_qp];
}

Real EnthalpyConvectionSteam::computeQpJacobian()
{

  //  return _darcy_mass_flux_steam[_qp]*_denthalpy_steamdH_P[_qp]*_grad_phi[_j][_qp]*_test[_i][_qp];
    return -_grad_test[_i][_qp]*
             ( _darcy_mass_flux_steam[_qp]*_denthalpy_steamdH_P[_qp]*_phi[_j][_qp]
              + _Ddarcy_mass_flux_steamDH[_qp]* _enthalpy_steam[_qp]*_phi[_j][_qp]);
}

Real EnthalpyConvectionSteam::computeQpOffDiagJacobian(unsigned int jvar)
{
  if(jvar==_p_var)
  {
    return  _grad_test[_i][_qp]*(_tau_steam[_qp]*_grad_phi[_j][_qp]*_enthalpy_steam[_qp]);
    }
    else 
    {
      return 0.0;
    }
  }

