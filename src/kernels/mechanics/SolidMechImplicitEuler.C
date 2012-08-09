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

#include "SolidMechImplicitEuler.h"

template<>
InputParameters validParams<SolidMechImplicitEuler>()
{
  //InputParameters params = validParams<SecondDerivativeImplicitEuler>();
  InputParameters params = validParams<TimeKernel>();
 
  return params;
}

SolidMechImplicitEuler::SolidMechImplicitEuler(const std::string & name, InputParameters parameters)
//:SecondDerivativeImplicitEuler(name, parameters),
    :TimeKernel(name, parameters),
     _u_old(valueOld()),
     _u_older(valueOlder()),
     _density(getMaterialProperty<Real>("density_rock"))
{}

Real
SolidMechImplicitEuler::computeQpResidual()
{
  //return _density[_qp]*SecondDerivativeImplicitEuler::computeQpResidual();
  return _density[_qp]*_test[_i][_qp]*((_u[_qp]-2*_u_old[_qp]+_u_older[_qp])/(_dt*_dt));
  
}

Real
SolidMechImplicitEuler::computeQpJacobian()
{
//  return _density[_qp]*SecondDerivativeImplicitEuler::computeQpJacobian();
  return _density[_qp]*_test[_i][_qp]*(_phi[_j][_qp]/(_dt*_dt));
  
}
  
