#include "GravityNeumannBC.h"

template<>
InputParameters valid_params<GravityNeumannBC>()
{
  InputParameters params;
  params.set<Real>("permeability") = 0;
  params.set<Real>("density") = 0;
  params.set<Real>("mu_w") = 0;
  return params;
}

GravityNeumannBC::GravityNeumannBC(std::string name, InputParameters parameters, std::string var_name, unsigned int boundary_id, std::vector<std::string> coupled_to, std::vector<std::string> coupled_as)
    :BoundaryCondition(name, parameters, var_name, true, boundary_id, coupled_to, coupled_as),
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

