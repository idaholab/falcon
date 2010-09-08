#include "PressureNeumannBC2.h"

template<>
InputParameters validParams<PressureNeumannBC2>()
{
  InputParameters params = validParams<BoundaryCondition>();
  params.set<Real>("component");
  return params;
}

PressureNeumannBC2::PressureNeumannBC2(const std::string & name, MooseSystem & moose_system, InputParameters parameters)
  :BoundaryCondition(name, moose_system, parameters),
    _pe_var(coupled("pe")),
    _pe(coupledValue("pe")),
    _u_vel_var(coupled("u")),
    _u_vel(coupledValue("u")),
    _v_vel_var(coupled("v")),
    _v_vel(coupledValue("v")),
    _w_vel_var(_dim == 3 ? coupled("w") : 0),
    _w_vel(_dim == 3 ? coupledValue("w") : _zero),
    _component(getParam<Real>("component")),
    _gamma(getMaterialProperty<Real>("gamma"))
{
  if(_component < 0)
  {
    std::cout<<"Must select a component for PressureNeumannBC2"<<std::endl;
    libmesh_error();
  }
}

Real
PressureNeumannBC2::pressure()
{
  return (_gamma[_qp] - 1)*(_pe[_qp] - (0.5 * (_u_vel[_qp]*_u_vel[_qp] + _v_vel[_qp]*_v_vel[_qp] + _w_vel[_qp]*_w_vel[_qp])));
}

Real
PressureNeumannBC2::computeQpResidual()
{
  return pressure()*_normals[_qp](_component)*_phi[_i][_qp];
}
