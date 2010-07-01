#include "OutFlowBC.h"

template<>
InputParameters validParams<OutFlowBC>()
{
  InputParameters params = validParams<BoundaryCondition>();
  params.set<Real>("thermal_conductivity") = 5.0;
//  params.set<Real>("conductivity")= 0.0;
  params.set<Real>("porosity")    = 1.0;
  return params;
}

OutFlowBC::OutFlowBC(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :BoundaryCondition(name, moose_system, parameters),
//   _grad_p(coupledGradient("p")),
//   _cond(parameters.get<Real>("conductivity")),
   _diff(parameters.get<Real>("thermal_conductivity")),
   _porosity(parameters.get<Real>("porosity"))
{}

Real
OutFlowBC::computeQpResidual()
  {
//    RealGradient _Darcy_vel = -_cond*_grad_p[_qp];
//    std::cout<<"Darcy velocity" << _Darcy_vel(1);
//    std::cout << "porosity,diffusivity, cond " << _porosity <<" " << _diff <<" "<< _cond << std::endl;

    Real _aa = -_phi[_i][_qp]*_porosity*_diff*_grad_u[_qp]*_normals[_qp];
    
//    if (_aa <= 1.0e-12)
//      _aa=0.0;
    
//    std::cout << "utlet_flux " << _aa << std::endl;
    
    return _aa;
  }


Real
OutFlowBC::computeQpJacobian()
  {
//    RealGradient _Darcy_vel = -_cond*_grad_p[_qp];
    return -_phi[_i][_qp]*_porosity*_diff*_grad_phi[_j][_qp]*_normals[_qp];
  }
