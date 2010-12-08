#include "SolidMechYFalcon.h"

template<>
InputParameters validParams<SolidMechYFalcon>()
{
  InputParameters params = validParams<SolidMechFalcon>();
  params.addRequiredCoupledVar("x", "Coupled Displacement in the x Direction");
  params.addCoupledVar("z", "Coupled Displacement in the z Direction");
  return params;
}

SolidMechYFalcon::SolidMechYFalcon(const std::string & name, InputParameters parameters)
  :SolidMechFalcon(name, parameters),
   _x_var(coupled("x")),
   _x(coupledValue("x")),
   _grad_x(coupledGradient("x")),
   _z_var(_dim == 3 ? coupled("z") : 1000000),
   _z(_dim == 3 ? coupledValue("z") : _zero),
   _grad_z(_dim == 3 ? coupledGradient("z") : _grad_zero)
{}

Real
SolidMechYFalcon::computeQpResidual()
{
/*    
      recomputeConstants();

      _strain(0,0) = _grad_x[_qp](0);
      _strain(1,1) = _grad_u[_qp](1);
      _strain(0,1) = _grad_x[_qp](1)+_grad_u[_qp](0);

      _stress(0) = _c1*_c3*_strain(0,1);
      _stress(1) = _c1*_c2*_strain(0,0)+_c1*_strain(1,1);      _stress(0) = ( *_stress_normal_vector)[_qp](0); //tau_xx
      _stress(1) =  ( *_stress_shear_vector)[_qp](0);  //tau_xy
      if( 3 == _dim)
      _stress(2) =  ( *_stress_shear_vector)[_qp](1);  //tau_xz


      if( 3 == _dim){
      _strain(2,2) = _grad_z[_qp](2);
      _strain(1,2) = _grad_z[_qp](1)+_grad_u[_qp](2);

      _stress(1) += _c1*_c2*_strain(2,2);
      _stress(2) = _c1*_c3*_strain(1,2);
      }
*/
  _stress(0) = _stress_shear_vector[_qp](0); //tau_yx
  _stress(1) = _stress_normal_vector[_qp](1); //tau_yy
  if( 3 == _dim)
    _stress(2) = _stress_shear_vector[_qp](2); //tau_yz

  Real value = (_stress*_grad_phi[_i][_qp]);
    
  return value;
}

Real
SolidMechYFalcon::computeQpJacobian()
{
  recomputeConstants();

  Real value = _c1*(_grad_phi[_i][_qp]*(_B22*_grad_phi[_j][_qp]));

  return value;
}

Real
SolidMechYFalcon::computeQpOffDiagJacobian(unsigned int jvar)
{
  recomputeConstants();
    
  RealGradient value = 0;

  if(jvar == _x_var)
    value += _B21*_grad_phi[_j][_qp];
  else if(jvar == _z_var)
    value += _B23*_grad_phi[_j][_qp];

  return _c1*(_grad_phi[_i][_qp]*value);
}
