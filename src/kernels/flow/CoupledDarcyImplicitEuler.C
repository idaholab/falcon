
#include "CoupledDarcyImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<CoupledDarcyImplicitEuler>()
{
     InputParameters params = validParams<ImplicitEuler>();
     params.addCoupledVar("enthalpy", "TODO: add description");
     params.addCoupledVar("densityAux", "TODO: add description");
     return params;
}

CoupledDarcyImplicitEuler::CoupledDarcyImplicitEuler(const std::string & name, InputParameters parameters)
  :ImplicitEuler(name, parameters),

//     _density(coupledValue("densityAux")),
//     _density_old(coupledValueOld("densityAux")),

   _density(getMaterialProperty<Real>("density")),
   _density_old(getMaterialProperty<Real>("density_old")),

   _enthalpy(coupledValue("enthalpy")),
   _porosity(getMaterialProperty<Real>("porosity")),
   _S_water(getMaterialProperty<Real>("S_water")),
   _S_steam(getMaterialProperty<Real>("S_steam")),
   _enthalpy_saturated_water(getMaterialProperty<Real>("enthalpy_saturated_water")),
   _enthalpy_saturated_steam(getMaterialProperty<Real>("enthalpy_saturated_steam"))
   
        
{
     E3 = pow(10,3);
     E6 = pow(10,6);
     
     a2 = 4.00894*pow(10,-4);
     a5 = 5.46283*pow(10,-7);
     
     b2 = 0.0438441;
     b3 = 1.79088*pow(10,-5);
     b4 = 3.69276*pow(10,-8);
     b5 = 5.17644*pow(10,-13);
}

Real
CoupledDarcyImplicitEuler::computeQpResidual()
{ 

     return _porosity[_qp]*_test[_i][_qp]*(_density[_qp] -_density_old[_qp])/_dt;
}

Real
CoupledDarcyImplicitEuler::computeQpJacobian()
{    
  Real H = _enthalpy[_qp]/E3;
  Real P = _u[_qp]/E6;
  Real P2 = pow(P,2);  
  Real P3 = pow(P,3);
  Real P4 = pow(P,4);
  Real H2 = pow(H,2);
  Real H3 = pow(H,3);
  
  Real  ddensitydp;
  Real  ddensity_steam_dp = 0;
  Real  ddensity_water_dp = 0;

  Real phij = _phi[_j][_qp];
     
  //   compressed water zone
  if (H < _enthalpy_saturated_water[_qp])
  {
    if (H >200.0)
      ddensity_water_dp = E3*((a2*phij)+(a5*phij*H));
  }
  

  // Super heated steam zone
  else if (H > _enthalpy_saturated_steam[_qp])
  {
    ddensity_steam_dp = E3*((b2*phij)-(b3*phij*H)+(b4*4*P3*phij)+(b5*phij*H3));
  }

  // Mixed phase
  else 
  {
    ddensity_water_dp = E3*((a2*phij)+(a5*phij*_enthalpy_saturated_water[_qp]));
    ddensity_steam_dp = E3*((b2*phij)-(b3*phij*_enthalpy_saturated_steam[_qp])+(b4*4*P3*phij)+(b5*phij*pow(_enthalpy_saturated_steam[_qp],3)));
  }

  ddensitydp = (_S_water[_qp]*ddensity_water_dp)+(_S_steam[_qp]*ddensity_steam_dp);

  return _porosity[_qp]*_test[_i][_qp]*ddensitydp/_dt;   
}
