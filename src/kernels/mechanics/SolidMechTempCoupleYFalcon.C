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

#include "SolidMechTempCoupleYFalcon.h"

template<>
InputParameters validParams<SolidMechTempCoupleYFalcon>()
{
  InputParameters params = validParams<SolidMechTempCoupleFalcon>();
  params.addCoupledVar("temperature", "TODO: add description");
  return params;
}

SolidMechTempCoupleYFalcon::SolidMechTempCoupleYFalcon(const std::string & name, InputParameters parameters)
  :SolidMechTempCoupleFalcon(name, parameters)
{}

Real
SolidMechTempCoupleYFalcon::computeQpResidual()
{
  recomputeCouplingConstants();

  return -(_c1*(1+2*_c2)*_grad_test[_i][_qp](1)*_thermal_strain[_qp]);
}

Real
SolidMechTempCoupleYFalcon::computeQpOffDiagJacobian(unsigned int jvar)
{
  recomputeCouplingConstants();

  if(jvar == _temp_var)
    return -(_c1*(1+2*_c2)*_grad_test[_i][_qp](1)*_alpha[_qp]*_phi[_j][_qp]);

  return 0.0;
}
