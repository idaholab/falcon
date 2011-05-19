#include "StressStrainDamageComputeAux.h"

template<>
InputParameters validParams<StressStrainDamageComputeAux>()
{
     InputParameters params = validParams<AuxKernel>();

     params.addParam<std::string>("quantity","stress", "output: stress, strain, or damage.");     
     params.addParam<int>("component",0,"component of the stress/strain vector, 0-xx;1-yy;2-zz;3-xy;4-xz;5-yz");
     
     return params;
}

StressStrainDamageComputeAux::StressStrainDamageComputeAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
   _quantity_string( getParam<std::string>("quantity") ),
   _component(getParam<int>("component")),
   _damage_coeff(getMaterialProperty<Real>("damage_coeff")),   
   _stress_normal_vector(getMaterialProperty<RealVectorValue>("stress_normal_vector")),
   _stress_shear_vector(getMaterialProperty<RealVectorValue>("stress_shear_vector")),
   _strain_normal_vector(getMaterialProperty<RealVectorValue>("strain_normal_vector")),
   _strain_shear_vector(getMaterialProperty<RealVectorValue>("strain_shear_vector"))
{}

Real
StressStrainDamageComputeAux::computeValue()
{

                                //damage factor
  std::transform(_quantity_string.begin(), _quantity_string.end(),
                 _quantity_string.begin(), ::tolower);
  if ( _quantity_string == "stress" )
  {
    _stress(0) = _stress_normal_vector[_qp](0);                //tau_xx
    _stress(1) = _stress_normal_vector[_qp](1);                //tau_yy
    if (_dim == 3) _stress(2) = _stress_normal_vector[_qp](2); //tau_zz
    _stress(3) = _stress_shear_vector[_qp](0);                  //tau_xy

    if (_dim == 3)
    {
      _stress(4) =  _stress_shear_vector[_qp](1);               //tau_xz
      _stress(5) =  _stress_shear_vector[_qp](2);                //tau_yz
    }
    return _stress(_component);
      
  }
  else if ( _quantity_string == "strain" )
  {
    _strain(0) = _strain_normal_vector[_qp](0);                //s_xx
    _strain(1) = _strain_normal_vector[_qp](1);                //s_yy
    if (_dim == 3) _strain(2) = _strain_normal_vector[_qp](2); //s_zz
    _strain(3) = _strain_shear_vector[_qp](0);                  //s_xy

    if (_dim == 3)
    {
      _strain(4) =  _strain_shear_vector[_qp](1);               //s_xz
      _strain(5) =  _strain_shear_vector[_qp](2);               //s_yz
    }
    return _strain(_component);
  }
  else if ( _quantity_string == "damage" )
  {
    _damage = _damage_coeff[_qp];                              //damage factor
    return _damage;
  }
  return 0.0;
  
}
