#include "Material.h"
#include "EnthalpyConvectionWater.h"

template<>
InputParameters validParams<EnthalpyConvectionWater>()
{
    InputParameters params = validParams<Kernel>();
    params.addRequiredCoupledVar("enthalpy_water", "Use CoupledAuxwater enthalpy here");
    params.addRequiredCoupledVar("denthalpy_waterdH_P", "Use CoupledAux dsteamenthalpydh_P here");
    return params;
}

EnthalpyConvectionWater::EnthalpyConvectionWater(const std::string & name, InputParameters parameters)
:Kernel(name, parameters),

_darcy_mass_flux_water(getMaterialProperty<RealGradient>("darcy_mass_flux_water")),
_enthalpy_water(coupledValue("enthalpy_water")),
_denthalpy_waterdH_P(coupledValue("denthalpy_waterdH_P"))
{}

Real EnthalpyConvectionWater::computeQpResidual()
{
    
    
   // return  _darcy_mass_flux_water[_qp]*_grad_enthalpy_water[_qp]*_test[_i][_qp];
  return  -_darcy_mass_flux_water[_qp]*_enthalpy_water[_qp]*_grad_test[_i][_qp]; 
}

Real EnthalpyConvectionWater::computeQpJacobian()
{
    
    //return _darcy_mass_flux_water[_qp]*_denthalpy_waterdH_P[_qp]*_grad_phi[_j][_qp]*_test[_i][_qp];
    return -_darcy_mass_flux_water[_qp]*_denthalpy_waterdH_P[_qp]*_phi[_j][_qp]*_grad_test[_i][_qp];
    
}
