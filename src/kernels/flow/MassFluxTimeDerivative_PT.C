#include "MassFluxTimeDerivative_PT.h"
#include "Material.h"

template<>
InputParameters validParams<MassFluxTimeDerivative_PT>()
{
  InputParameters params = validParams<TimeDerivative>();
   params.addRequiredCoupledVar("density_water", "Use Coupled density here to calculate the time derivative");
   params.addRequiredCoupledVar("dwdp", "derivative of water density vs temperature");
    return params;
}

MassFluxTimeDerivative_PT::MassFluxTimeDerivative_PT(const std::string & name, InputParameters parameters)
  :TimeDerivative(name, parameters),
   
   _density_water(coupledValue("density_water")),
   _density_water_old(coupledValueOld("density_water")),
   _dwdp(coupledValue("dwdp")),

   _porosity (getMaterialProperty<Real>("material_porosity"))
//   _porosity(coupledValue("porosity")),
//   _porosity_old(coupledValueOld("porosity"))

{}

Real
MassFluxTimeDerivative_PT::computeQpResidual()
{
//  std::cout << ((_porosity[_qp]*_density_water[_qp])-(_porosity_old[_qp]*_density_water_old[_qp]))/_dt << "\n";
 
  return (((_porosity[_qp]*_density_water[_qp])-(_porosity[_qp]*_density_water_old[_qp]))/_dt) * _test[_i][_qp];
}

Real
MassFluxTimeDerivative_PT::computeQpJacobian()
{  //std::cout <<"dwdp: "<< _dwdp[_qp]<<' '<<_density_water[_qp]<< "\n";
   Real tmp1 = (_porosity[_qp]*_dwdp[_qp]*_phi[_j][_qp])*_test[_i][_qp]/_dt;  
   return tmp1;
   
}
