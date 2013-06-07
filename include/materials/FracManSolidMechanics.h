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

#ifndef FRACMANSOLIDMECHANICS_H
#define FRACMANSOLIDMECHANICS_H

#include "FracManPorousMedia.h"
#include "ColumnMajorMatrix.h"
#include <cmath>

//libMesh includes
#include "libmesh/tensor_value.h"
#include "libmesh/vector_value.h"


//Forward Declarations
class FracManSolidMechanics;

template<>
InputParameters validParams<FracManSolidMechanics>();

/**
 * Simple material with SolidMechanics properties.
 */
class FracManSolidMechanics : virtual public FracManPorousMedia
{
public:
  FracManSolidMechanics(const std::string & name,
                 InputParameters parameters);

protected:

  void rotateSymmetricTensor( const ColumnMajorMatrix & , const RealTensorValue & ,
                                     RealTensorValue & );
  
  virtual void computeProperties();

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
  
  Real _matrix_thermal_expansion;
  Real _matrix_youngs_modulus;
  Real _matrix_t_ref;
    
  std::vector<int> _fracture_number_vec;
  std::vector<Real> _fracture_thermal_expansion_vec;
  std::vector<Real> _fracture_youngs_modulus_vec;
  std::vector<Real> _fracture_t_ref_vec;
    unsigned int num_frac_vec_entries;
    unsigned int num_te_vec_entries;
    unsigned int num_ym_vec_entries;
    unsigned int num_tref_vec_entries;
    
  Real _input_poissons_ratio;
  Real _input_biot_coeff;
  Real _input_biot_modulus;

  MaterialProperty<Real> & _thermal_strain;
  MaterialProperty<Real> & _alpha;
  MaterialProperty<Real> & _youngs_modulus;
  MaterialProperty<Real> & _poissons_ratio;
  MaterialProperty<Real> & _biot_coeff;
  MaterialProperty<Real> & _biot_modulus;

  MaterialProperty<RealVectorValue> & _stress_normal_vector;
  MaterialProperty<RealVectorValue> & _stress_shear_vector;
  MaterialProperty<RealVectorValue> & _strain_normal_vector;
  MaterialProperty<RealVectorValue> & _strain_shear_vector;

  Real _E;
  Real _nu;
  Real _c1;
  Real _c2;
  Real _c3;
};

#endif //FRACMANSOLIDMECHANICS_H
