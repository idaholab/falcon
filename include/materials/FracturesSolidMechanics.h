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

#ifndef FRACTURESSOLIDMECHANICS_H
#define FRACTURESSOLIDMECHANICS_H

#include "FracturesPorousMedia.h"

//Forward Declarations
class FracturesSolidMechanics;

template<>
InputParameters validParams<FracturesSolidMechanics>();

/**
 * Simple material with SolidMechanics properties.
 */
class FracturesSolidMechanics : virtual public FracturesPorousMedia
{
public:
  FracturesSolidMechanics(const std::string & name,
                 InputParameters parameters);

protected:
  virtual void computeProperties();
////Grab coupled variables
  bool _has_temp;
  VariableValue  & _temperature;
  bool _has_x_disp;
  VariableGradient & _grad_x_disp;
  VariableGradient & _grad_x_disp_old;
  bool _has_y_disp;
  VariableGradient & _grad_y_disp;
  VariableGradient & _grad_y_disp_old;
  bool _has_z_disp;
  VariableGradient & _grad_z_disp;
  VariableGradient & _grad_z_disp_old;

////Grab user input parameters
  //General
  Real _input_poissons_ratio;
  Real _input_biot_coeff;
  Real _input_biot_modulus;
  //Matrix
  Real _matrix_num;
  Real _matrix_thermal_expansion;
  Real _matrix_youngs_modulus;
  Real _matrix_t_ref;
  //Fractures
  Real _fracture_num;
  Real _fracture_thermal_expansion;
  Real _fracture_youngs_modulus;
  Real _fracture_t_ref;
  
///Declare material properties
  //rock material props
  MaterialProperty<Real> & _thermal_strain;
  MaterialProperty<Real> & _alpha;
  MaterialProperty<Real> & _youngs_modulus;
  MaterialProperty<Real> & _poissons_ratio;
  MaterialProperty<Real> & _biot_coeff;
  MaterialProperty<Real> & _biot_modulus;
  //stress/strain material props
  MaterialProperty<RealVectorValue> & _stress_normal_vector;
  MaterialProperty<RealVectorValue> & _stress_shear_vector;
  MaterialProperty<RealVectorValue> & _strain_normal_vector;
  MaterialProperty<RealVectorValue> & _strain_shear_vector;

////Local variables declared
  Real E;
  Real nu;
  Real c1;
  Real c2;
  Real c3;
};

#endif //FRACTURESSOLIDMECHANICS_H
