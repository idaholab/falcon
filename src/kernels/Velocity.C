#include "Velocity.h"
 
template<>
InputParameters validParams<Velocity>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

Velocity::Velocity(const std::string & name, MooseSystem & moose_system, InputParameters parameters)
  :Kernel(name, moose_system, parameters),
   _p_var(coupled("p")),
   _p(coupledValue("p")),
   _momentum_var(coupled("momentum")),
   _momentum(coupledValue("momentum"))
{}

Real
Velocity::computeQpResidual()
{
  return (_u[_qp]-(_momentum[_qp]/_p[_qp]))*_phi[_i][_qp];
}

Real
Velocity::computeQpJacobian()
{
  return _phi[_j][_qp]*_phi[_i][_qp];
}

Real
Velocity::computeQpOffDiagJacobian(unsigned int jvar)
{
  if(jvar == _p_var)
  {
    return (-(  ( _momentum[_qp]  /   (-_p[_qp]*_p[_qp])   ) * _phi[_j][_qp]))*_phi[_i][_qp];
  }
  else if(jvar == _momentum_var)
  {
    return (-(_phi[_j][_qp]/_p[_qp]))*_phi[_i][_qp];
  }

  return 0;
}
