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

#include "SolidMechZFalcon.h"

template<>
InputParameters validParams<SolidMechZFalcon>()
{
  InputParameters params = validParams<SolidMechFalcon>();
  params.addRequiredCoupledVar("x", "Coupled Displacement in the x Direction");
  params.addRequiredCoupledVar("y", "Coupled Displacement in the y Direction");
  return params;
}

SolidMechZFalcon::SolidMechZFalcon(const std::string & name, InputParameters parameters)
  :SolidMechFalcon(name, parameters),
   _x_var(coupled("x")),
   _x(coupledValue("x")),
   _grad_x(coupledGradient("x")),
   _y_var(coupled("y")),
   _y(coupledValue("y")),
   _grad_y(coupledGradient("y"))
{}

Real
SolidMechZFalcon::computeQpResidual()
{
/*
  recomputeConstants();

  _strain(0,0) = _grad_x[_qp](0);
  _strain(1,1) = _grad_y[_qp](1);
  _strain(2,2) = _grad_u[_qp](2);
  _strain(1,2) = _grad_u[_qp](1)+_grad_y[_qp](2);
  _strain(0,2) = _grad_x[_qp](2)+_grad_u[_qp](0);

  _stress(0) = _c1*_c3*_strain(0,2);
  _stress(1) = _c1*_c3*_strain(1,2);
  _stress(2) = _c1*_c2*_strain(0,0)+_c1*_c2*_strain(1,1)+_c1*_strain(2,2);
*/
  _stress(0) = _stress_shear_vector[_qp](1); //tau_zx
  _stress(1) = _stress_shear_vector[_qp](2); //tau_zy
  _stress(2) = _stress_normal_vector[_qp](2); //tau_zz

    
  return (_stress*_grad_test[_i][_qp]);

}

Real
SolidMechZFalcon::computeQpJacobian()
{
  recomputeConstants();

  Real value = _c1*(_grad_test[_i][_qp]*(_B33*_grad_phi[_j][_qp]));

  return value;
}

Real
SolidMechZFalcon::computeQpOffDiagJacobian(unsigned int jvar)
{
  recomputeConstants();
    
  RealGradient value = 0;

  if(jvar == _x_var)
    value += _B31*_grad_phi[_j][_qp];
  else if(jvar == _y_var)
    value += _B32*_grad_phi[_j][_qp];

  return _c1*(_grad_test[_i][_qp]*value);
}
