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

#include "TimeDerivative.h"

#ifndef PRESSURETIMEDERIVATIVE
#define PRESSURETIMEDERIVATIVE

//Forward Declarations
class PressureTimeDerivative;

template<>
InputParameters validParams<PressureTimeDerivative>();

class PressureTimeDerivative : public TimeDerivative
{
public:

    PressureTimeDerivative(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  bool _has_x_disp;
  VariableGradient & _grad_x_disp;
  VariableGradient & _grad_x_disp_old;
  bool _has_y_disp;
  VariableGradient & _grad_y_disp;
  VariableGradient & _grad_y_disp_old;
  bool _has_z_disp;
  VariableGradient & _grad_z_disp;
  VariableGradient & _grad_z_disp_old;
  bool _rigid_rock;

  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _compressibility;
  MaterialProperty<Real> & _biot_coeff;
  MaterialProperty<Real> & _biot_modulus;
//  VariableValue  & _porosity;
//  VariableValue  & _porosity_old;
  VariableValue & _u_old;
};
#endif //MASSFLUXTIMEDERIVATIVE
