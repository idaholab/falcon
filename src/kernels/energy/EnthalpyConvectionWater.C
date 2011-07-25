#include "Material.h"
#include "EnthalpyConvectionWater.h"

template<>
InputParameters validParams<EnthalpyConvectionWater>()
{
    InputParameters params = validParams<Kernel>();
    params.addRequiredCoupledVar("enthalpy_water", "Use CoupledAuxwater enthalpy here");
    params.addRequiredCoupledVar("denthalpy_waterdH_P", "Use CoupledAux dsteamenthalpydh_P here");

    params.addCoupledVar("pressure", "Use CoupledVar pressure here");
    return params;
}

EnthalpyConvectionWater::EnthalpyConvectionWater(const std::string & name, InputParameters parameters)
:Kernel(name, parameters),

_darcy_mass_flux_water(getMaterialProperty<RealGradient>("darcy_mass_flux_water")),
_Ddarcy_mass_flux_waterDH(getMaterialProperty<RealGradient>("Ddarcy_mass_flux_waterDH")),
 _tau_water(getMaterialProperty<Real>("tau_water")),
_enthalpy_water(coupledValue("enthalpy_water")),
 _denthalpy_waterdH_P(coupledValue("denthalpy_waterdH_P")),
 _p_var(coupled("pressure")),
 _p(coupledValue("pressure"))
{}

Real EnthalpyConvectionWater::computeQpResidual()
{
    
    
   // return  _darcy_mass_flux_water[_qp]*_grad_enthalpy_water[_qp]*_test[_i][_qp];
  return  -_darcy_mass_flux_water[_qp]*_enthalpy_water[_qp]*_grad_test[_i][_qp]; 
}

Real EnthalpyConvectionWater::computeQpJacobian()
{
    
    //return _darcy_mass_flux_water[_qp]*_denthalpy_waterdH_P[_qp]*_grad_phi[_j][_qp]*_test[_i][_qp];
    return -_grad_test[_i][_qp]*
            (_darcy_mass_flux_water[_qp]*_denthalpy_waterdH_P[_qp]*_phi[_j][_qp]
             +_Ddarcy_mass_flux_waterDH[_qp]*_enthalpy_water[_qp]*_phi[_j][_qp]);
    
}

Real EnthalpyConvectionWater::computeQpOffDiagJacobian(unsigned int jvar)
{
  if(jvar==_p_var)
  {return _grad_test[_i][_qp]*(_tau_water[_qp]*_grad_phi[_j][_qp]*_enthalpy_water[_qp]);
  }
  

  else
  {return 0.0;
  }
  
}
