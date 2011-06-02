#include "SolidMechXFalcon.h"

template<>
InputParameters validParams<SolidMechXFalcon>()
{
  InputParameters params = validParams<SolidMechFalcon>();
  params.addRequiredCoupledVar("y", "Coupled Displacement in the y Direction");
  params.addCoupledVar("z", "Coupled Displacement in the z Direction");
  return params;
}

SolidMechXFalcon::SolidMechXFalcon(const std::string & name, InputParameters parameters)
  :SolidMechFalcon(name, parameters),
   _y_var(coupled("y")),
   _y(coupledValue("y")),
   _grad_y(coupledGradient("y")),
   _z_var(_dim == 3 ? coupled("z") : 1000000),
   _z(_dim == 3 ? coupledValue("z") : _zero),
   _grad_z(_dim == 3 ? coupledGradient("z"): _grad_zero)
{}

Real
SolidMechXFalcon::computeQpResidual()
  {
/*    
    recomputeConstants();

    _strain(0,0) = _grad_u[_qp](0); //s_xx
    _strain(1,1) = _grad_y[_qp](1); //s_yy
    _strain(0,1) = _grad_u[_qp](1)+_grad_y[_qp](0); //2.0*sxy

    _stress(0) = _c1*_strain(0,0)+_c1*_c2*_strain(1,1); //tau_xx
    _stress(1) = _c1*_c3*_strain(0,1);                  //tau_yx

    if( 3 == _dim){
      _strain(2,2) = _grad_z[_qp](2); //s_zz
      _strain(0,2) = _grad_u[_qp](2)+_grad_z[_qp](0);  //2.0*s_zx

      _stress(0) += _c1*_c2*_strain(2,2);
      _stress(2) = _c1*_c3*_strain(0,2);   //tau_zx
    }
*/
      _stress(0) = _stress_normal_vector[_qp](0); //tau_xx
      _stress(1) = _stress_shear_vector[_qp](0); //tau_xy
    if( 3 == _dim)
      _stress(2) = _stress_shear_vector[_qp](1); //tau_xz
    
    Real value = (_stress*_grad_test[_i][_qp]);
    
    return value;
  }

Real
SolidMechXFalcon::computeQpJacobian()
  {
    recomputeConstants();

    Real value = _c1*(_grad_test[_i][_qp]*(_B11*_grad_phi[_j][_qp]));

    return value;
  }


Real
SolidMechXFalcon::computeQpOffDiagJacobian(unsigned int jvar)
  {
    recomputeConstants();
    
    RealGradient value = 0;

    if(jvar == _y_var)
      value += _B12*_grad_phi[_j][_qp];
    else if(jvar == _z_var)
      value += _B13*_grad_phi[_j][_qp];

    return _c1*(_grad_test[_i][_qp]*value);
  }
