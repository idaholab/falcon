#ifndef SOLIDMECHANICS_H
#define SOLIDMECHANICS_H

#include "PorousMedia.h"
#include "ColumnMajorMatrix.h"

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
  static void rotateSymmetricTensor( const ColumnMajorMatrix & , const RealTensorValue & ,
                                     RealTensorValue & );

  void computeDamage(const int qp); //damage mechanics
  void computeCrack_tension(const int qp); //tensile induce cracking
  void computeCrack_Mohr_Coulomb_v1(const int qp); //Mohr_Coulomb criteria
  void computeCrack_Mohr_Coulomb_v2(const int qp); //Mohr_Coulomb criteria
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
  Real _input_t_ref;
  
  bool _has_damage;
  Real _input_damage_coeff;             //initial damage between [0,1]
  Real _input_strain_initialize_damage; //critical strain to lnitialize damage
  Real _input_strain_broken;            //critical strain for complete failure
  Real _damage_a1;                      //parameters for youngs modulus and damage factor
  Real _damage_a2;

  bool _has_crack;
  std::string _has_crack_method; // true <==> tensile_induced_crack  false<==>Mohr_Coulomb
  Real _critical_crack_strain;
  Real _cohesion;
  Real _friction_angle;

  ColumnMajorMatrix _total_strain;
  TensorValue<Real> _total_stress;
  ColumnMajorMatrix _total_stress1;
  
  MaterialProperty<Real> & _thermal_strain;
  MaterialProperty<Real> & _alpha;
  MaterialProperty<Real> & _youngs_modulus;
  MaterialProperty<Real> & _poissons_ratio;
  MaterialProperty<Real> & _biot_coeff;
  MaterialProperty<Real> & _damage_coeff;
  MaterialProperty<Real> & _strain_history;

  MaterialProperty<Real> & _damage_coeff_old;//newly added
  MaterialProperty<Real> & _strain_history_old;//newly added
  

  MaterialProperty<RealVectorValue> & _stress_normal_vector;
  MaterialProperty<RealVectorValue> & _stress_shear_vector;
  MaterialProperty<RealVectorValue> & _strain_normal_vector;
  MaterialProperty<RealVectorValue> & _strain_shear_vector;

  MaterialProperty<RealVectorValue> * _crack_flags;
  MaterialProperty<RealVectorValue> * _crack_flags_old;
  
  Real _E;
  Real _nu;
  Real _c1;
  Real _c2;
  Real _c3;
};

#endif //SOLIDMECHANICS_H
