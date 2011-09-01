#ifndef SOLIDMECHANICS_H
#define SOLIDMECHANICS_H

#include "PorousMedia.h"
#include "ColumnMajorMatrix.h"
#include <cmath>

//libMesh includes
#include "tensor_value.h"
#include "vector_value.h"


//Forward Declarations
class SolidMechanics;

template<>
InputParameters validParams<SolidMechanics>();

/**
 * Simple material with SolidMechanics properties.
 */
class SolidMechanics : virtual public PorousMedia
{
public:
  SolidMechanics(const std::string & name,
                 InputParameters parameters);

protected:
//   static Real randn_trig(Real mu=0.0,Real sigma=1.0)
//   {
//     static bool deviateAvailable=false;
//     static Real storedDeviate;
//     Real dist, angle, PI=3.14159265;
//     if(!deviateAvailable)
//     {
//       dist=std::sqrt(-2.0 * std::log(double(rand()) / double(RAND_MAX)) );
//       angle=2.0 * PI * (double(rand()) / double(RAND_MAX));
//       storedDeviate=dist*std::cos(angle);
//       deviateAvailable=true;
//       return dist * std::sin(angle) * sigma + mu;
//     }
//     else
//     {
//       deviateAvailable=false;
//       return storedDeviate * sigma + mu;
//     }
//   }

  void rotateSymmetricTensor( const ColumnMajorMatrix & , const RealTensorValue & ,
                                     RealTensorValue & );
  void computeCrack_tension(const int qp); //tensile induce cracking
  virtual void computeProperties();

  bool _has_temp;
  VariableGradient & _grad_temp;
  VariableValue  & _temperature;

  bool _has_x_disp;
  VariableGradient & _grad_x_disp;
  bool _has_y_disp;
  VariableGradient & _grad_y_disp;
  bool _has_z_disp;
  VariableGradient & _grad_z_disp;

  Real _input_thermal_expansion;
  Real _input_youngs_modulus;
  Real _input_poissons_ratio;

  Real _input_biot_coeff;
  Real _input_biot_modulus;
  Real _input_t_ref;
//parameters for smear crack model
  bool _has_crack;
  std::string _has_crack_method; // true <==> tensile_induced_crack  false<==>Mohr_Coulomb
  Real _critical_crack_strain;
  Real _cohesion;
  Real _friction_angle;

  bool _has_damage_couple_permeability;
  Real _damage_couple_permeability_coeff1;
  Real _damage_couple_permeability_coeff2;

  ColumnMajorMatrix _total_strain;
  TensorValue<Real> _total_stress;
  ColumnMajorMatrix _total_stress1;

  MaterialProperty<Real> & _thermal_strain;
  MaterialProperty<Real> & _alpha;
  MaterialProperty<Real> & _youngs_modulus;
  MaterialProperty<Real> & _poissons_ratio;
  MaterialProperty<Real> & _biot_coeff;
  MaterialProperty<Real> & _biot_modulus;
  MaterialProperty<Real> & _damage_coeff;
  MaterialProperty<Real> & _damage_coeff_old;//newly added

  MaterialProperty<RealVectorValue> & _stress_normal_vector;
  MaterialProperty<RealVectorValue> & _stress_shear_vector;
  MaterialProperty<RealVectorValue> & _strain_normal_vector;
  MaterialProperty<RealVectorValue> & _strain_shear_vector;
  MaterialProperty<RealVectorValue> & _pstress_normal_vector;
  MaterialProperty<RealVectorValue> & _pstrain_normal_vector;

  MaterialProperty<RealVectorValue> * _crack_flags;
  MaterialProperty<RealVectorValue> * _crack_flags_old;

  Real _E;
  Real _nu;
  Real _c1;
  Real _c2;
  Real _c3;
};

#endif //SOLIDMECHANICS_H
