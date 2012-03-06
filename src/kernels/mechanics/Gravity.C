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

#include "Gravity.h"

template<>
InputParameters validParams<Gravity>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<int>("component",2,"component of the pressure vector");
  return params;
}

Gravity::Gravity(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _component(getParam<int>("component")),
   _density(getMaterialProperty<Real>("density_rock")),
   _gravity(getMaterialProperty<Real>("gravity")),
   _gravity_vector(getMaterialProperty<RealVectorValue>("gravity_vector"))
{}

Real
Gravity::computeQpResidual()
{
  return _test[_i][_qp] * _density[_qp]*_gravity[_qp]*_gravity_vector[_qp](_component);
}

Real
Gravity::computeQpJacobian()
{
  return 0.0;
}
  
