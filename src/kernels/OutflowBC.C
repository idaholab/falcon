#include "OutFlowBC.h"

template<>
InputParameters validParams<OutFlowBC>()
{
  InputParameters params;
  params.set<Real>("water_specific_heat") = 0;
  params.set<Real>("rho_w") = 0;

  return params;
}

OutFlowBC::OutFlowBC(std::string name, InputParameters parameters, std::string var_name, unsigned int boundary_id, std::vector<std::string> coupled_to, std::vector<std::string> coupled_as)
  :BoundaryCondition(name, parameters, var_name, true, boundary_id, coupled_to, coupled_as),
   _water_specific_heat(_parameters.get<Real>("water_specific_heat")),
   _rho_w(_parameters.get<Real>("rho_w")),
   _darcy_velocity(0,0,-9.e-4)
   
{}



Real
OutFlowBC::computeQpResidual()
  {
//    Real _aa = -_phi_face[_i][_qp]*(_water_specific_heat)*(_darcy_velocity)*(_rho_w)*_grad_u_face[_qp]*_normals_face[_qp];
    return -_phi_face[_i][_qp]*(
      ((_water_specific_heat)*(_rho_w))*_grad_u_face[_qp]*_normals_face[_qp]);
    

  }


Real
OutFlowBC::computeQpJacobian()
  {

    return -_phi_face[_i][_qp]*(
      ((_water_specific_heat)*(_rho_w))*_dphi_face[_j][_qp]*_normals_face[_qp]);
  }
