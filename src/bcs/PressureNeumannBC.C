#include "PressureNeumannBC.h"

template<>
InputParameters validParams<PressureNeumannBC>()
{
  InputParameters params = validParams<BoundaryCondition>();
  params.set<Real>("component");
  return params;
}

PressureNeumannBC::PressureNeumannBC(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :BoundaryCondition(name, moose_system, setIntegratedParam(parameters, true)),
    _pe_var(coupled("pe")),
    _pe(coupledValFace("pe")),
    _u_vel_var(coupled("u")),
    _u_vel(coupledValFace("u")),
    _v_vel_var(coupled("v")),
    _v_vel(coupledValFace("v")),
    _w_vel_var(_dim == 3 ? coupled("w") : 0),
    _w_vel(_dim == 3 ? coupledValFace("w") : _zero),
    _component(parameters.get<Real>("component"))
  {
    if(_component < 0)
    {
      std::cout<<"Must select a component for PressureNeumannBC"<<std::endl;
      libmesh_error();
    }
  }

Real
PressureNeumannBC::pressure()
  {
    //Only CONSTANT Real properties can be used by BCs
    Real gamma = _material->getConstantRealProperty("gamma");

    return (gamma - 1)*(_pe[_qp] - (0.5 * (_u_vel[_qp]*_u_vel[_qp] + _v_vel[_qp]*_v_vel[_qp] + _w_vel[_qp]*_w_vel[_qp])));
  }

Real
PressureNeumannBC::computeQpResidual()
  {
    return pressure()*_normals[_qp](_component)*_phi[_i][_qp];
  }
