/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "GravityNeumannBC.h"

template<>
InputParameters validParams<GravityNeumannBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  return params;
}

GravityNeumannBC::GravityNeumannBC(const std::string & name, InputParameters parameters)
  :IntegratedBC(name, parameters),
    _permeability(getMaterialProperty<Real>("permeability")),
    _density_water(getMaterialProperty<Real>("density_water")),
    _viscosity_water(getMaterialProperty<Real>("viscosity_water")),
    _gravity(getMaterialProperty<Real>("gravity")),
    _gravity_vector(getMaterialProperty<RealVectorValue>("gravity_vector"))
{}

Real
GravityNeumannBC::computeQpResidual()
{
  return _test[_i][_qp]*(-(((_permeability[_qp]*_density_water[_qp])/_viscosity_water[_qp])*_grad_u[_qp]*_normals[_qp])
    -(((_permeability[_qp]*_density_water[_qp]*_density_water[_qp])/_viscosity_water[_qp])*_gravity[_qp]*_gravity_vector[_qp]*_normals[_qp])
    );
}

Real
GravityNeumannBC::computeQpJacobian()
{
  return _test[_i][_qp]*(-(((_permeability[_qp]*_density_water[_qp])/_viscosity_water[_qp])*_grad_phi[_j][_qp]*_normals[_qp]));
}

