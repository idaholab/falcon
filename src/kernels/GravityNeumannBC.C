#include "GravityNeumannBC.h"

template<>
InputParameters validParams<GravityNeumannBC>()
{
  InputParameters params = validParams<BoundaryCondition>();
  params.set<Real>("permeability") = 0;
  params.set<Real>("density") = 0;
  params.set<Real>("mu_w") = 0;
  return params;
}

GravityNeumannBC::GravityNeumannBC(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :BoundaryCondition(name, moose_system, parameters),
     _permeability(_parameters.get<Real>("permeability")),
     _density(_parameters.get<Real>("density")),
     _mu_w(_parameters.get<Real>("mu_w")),
     _gravity(0,0,-9.8065)
{}

Real
GravityNeumannBC::computeQpResidual()
{
  return _phi_face[_i][_qp]*(
    -(((_permeability*_density)/_mu_w)*_grad_u_face[_qp]*_normals_face[_qp])
    -
    (((_permeability*_density*_density)/_mu_w)*_gravity*_normals_face[_qp])
    );
}

Real
GravityNeumannBC::computeQpJacobian()
{
  return _phi_face[_i][_qp]*(
    -(((_permeability*_density)/_mu_w)*_dphi_face[_j][_qp]*_normals_face[_qp]));  
}

