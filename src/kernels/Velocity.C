#include "Velocity.h"
 

template<>
Parameters valid_params<Velocity>()
{
  Parameters params;
  return params;
}

Velocity::Velocity(std::string name,
                  Parameters parameters,
                  std::string var_name,
                  std::vector<std::string> coupled_to,
                  std::vector<std::string> coupled_as)
    :Kernel(name,parameters,var_name,true,coupled_to,coupled_as),
    _p_var(coupled("p")),
    _p(coupledVal("p")),
    _momentum_var(coupled("momentum")),
    _momentum(coupledVal("momentum"))
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
