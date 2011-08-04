#include "PressureTimeDerivative.h"
#include "Material.h"

template<>
InputParameters validParams<PressureTimeDerivative>()
{
    InputParameters params = validParams<TimeDerivative>();
    return params;
}

PressureTimeDerivative::PressureTimeDerivative(const std::string & name, InputParameters parameters)
:TimeDerivative(name, parameters),

_porosity (getMaterialProperty<Real>("material_porosity")),
_compressibility(getMaterialProperty<Real>("compressibility"))
//   _porosity(coupledValue("porosity")),
//   _porosity_old(coupledValueOld("porosity"))

{}

Real
PressureTimeDerivative::computeQpResidual()
{
//    std::cout << _porosity[_qp] << " " << _compressibility[_qp] << "\n";
    return (_compressibility[_qp]*((_porosity[_qp]*_u[_qp])-(_porosity[_qp]*_u_old[_qp]))/_dt) * _test[_i][_qp];
}

Real
PressureTimeDerivative::computeQpJacobian()
{ 
    // std::cout << _ddensitydp_H[_qp]<<"\n" ;
    Real tmp1 = (_porosity[_qp]*_compressibility[_qp]*_phi[_j][_qp])*_test[_i][_qp]/_dt;  
    return tmp1;
}
