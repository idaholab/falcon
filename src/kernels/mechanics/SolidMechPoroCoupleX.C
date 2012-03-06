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

#include "SolidMechPoroCoupleX.h"

template<>
InputParameters validParams<SolidMechPoroCoupleX>()
{
  InputParameters params = validParams<SolidMechPoroCouple>();
  params.addRequiredCoupledVar("pressure", "Coupled Pressure");
  return params;
}

SolidMechPoroCoupleX::SolidMechPoroCoupleX(const std::string & name, InputParameters parameters)
  :SolidMechPoroCouple(name, parameters)
   
{}

Real
SolidMechPoroCoupleX::computeQpResidual()
{
//  return -_grad_test[_i][_qp](0)*_biot_coeff[_qp]*_pressure_val[_qp];
  return _test[_i][_qp]*_biot_coeff[_qp]*_grad_pressure[_qp](0);
  
}

Real
SolidMechPoroCoupleX::computeQpOffDiagJacobian(unsigned int jvar)
{
  if(jvar == _pressure_var)
//    return -_grad_test[_i][_qp](0)*_biot_coeff[_qp]*_phi[_j][_qp];
    return _test[_i][_qp]*_biot_coeff[_qp]*_grad_phi[_j][_qp](0);
  return 0.0;
}
