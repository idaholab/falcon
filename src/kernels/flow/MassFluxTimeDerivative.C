#include "MassFluxTimeDerivative.h"
#include "Material.h"
#include "Water_Steam_EOS.h"
template<>
InputParameters validParams<MassFluxTimeDerivative>()
{
  InputParameters params = validParams<TimeDerivative>();
   params.addRequiredCoupledVar("density", "Use Coupled density here to calculate the time derivative");
   params.addRequiredCoupledVar("ddensitydp_H", "derivative of water density vs temperature");
   params.addCoupledVar("enthalpy"," Use Coupled enthalpy to calculate the time derivative");
    return params;
}

MassFluxTimeDerivative::MassFluxTimeDerivative(const std::string & name, InputParameters parameters)
  :TimeDerivative(name, parameters),
   
   _density(coupledValue("density")),
   _density_old(coupledValueOld("density")),
   _ddensitydp_H(coupledValue("ddensitydp_H")),
   _enthalpy_old(coupledValueOld("enthalpy")),
   _porosity (getMaterialProperty<Real>("material_porosity"))
//   _porosity(coupledValue("porosity")),
//   _porosity_old(coupledValueOld("porosity"))

{}

Real
MassFluxTimeDerivative::computeQpResidual()
{
  //std::cout << _density[_qp] << " " << _density_old[_qp] << "\n";
  Real _temp[11];
  int _name;
  Real _den_mix;
  Real _den_old;
  
  
  if (_t_step==1)
  {
   
    
     Water_Steam_EOS::water_steam_prop_ph_(_u_old[_qp], _enthalpy_old[_qp] , _temp[0], 
                                              _temp[1], _den_mix, 
                                           _temp[2],_temp[3] ,_temp[4],_temp[5],_temp[6],_temp[7],
                                           _temp[8],_temp[9],_temp[10],_temp[11],_name
                                              ); 
        
        _den_old=_den_mix; 
    } 
    else 
    { _den_old= _density_old[_qp];} 


      
  return (((_porosity[_qp]*_density[_qp])-(_porosity[_qp]*_den_old))/_dt) * _test[_i][_qp];
}

Real
MassFluxTimeDerivative::computeQpJacobian()
{ 
   // std::cout << _ddensitydp_H[_qp]<<"\n" ;
   Real tmp1 = (_porosity[_qp]*_ddensitydp_H[_qp]*_phi[_j][_qp])*_test[_i][_qp]/_dt;  
    return tmp1;
}
