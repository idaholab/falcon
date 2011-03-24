#include "GravityNeumannBC.h"

template<>
InputParameters validParams<GravityNeumannBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.set<Real>("permeability") = 0;
  params.set<Real>("density") = 0;
  params.set<Real>("viscosity_water") = 0;
  return params;
}

GravityNeumannBC::GravityNeumannBC(const std::string & name, InputParameters parameters)
  :IntegratedBC(name, parameters),
     _permeability(getParam<Real>("permeability")),
     _density(getParam<Real>("density")),
     _viscosity_water(getParam<Real>("viscosity_water")),
     _gravity(0,0,-9.8065)
{}

Real
GravityNeumannBC::computeQpResidual()
{
  return _phi[_i][_qp]*(
    -(((_permeability*_density)/_viscosity_water)*_grad_u[_qp]*_normals[_qp])
    -
    (((_permeability*_density*_density)/_viscosity_water)*_gravity*_normals[_qp])
    );
}

Real
GravityNeumannBC::computeQpJacobian()
{
  return _phi[_i][_qp]*(
    -(((_permeability*_density)/_viscosity_water)*_grad_phi[_j][_qp]*_normals[_qp]));
}

