#include "SolidMechFalcon.h"

template<>
InputParameters validParams<SolidMechFalcon>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

SolidMechFalcon::SolidMechFalcon(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _E_prop(getMaterialProperty<Real>("youngs_modulus")),
   _nu_prop(getMaterialProperty<Real>("poissons_ratio")),
   _stress_normal_vector(getMaterialProperty<RealVectorValue>("stress_normal_vector")),
   _stress_shear_vector(getMaterialProperty<RealVectorValue>("stress_shear_vector"))
{}

void
SolidMechFalcon::recomputeConstants()
{
  _E = _E_prop[_qp];
  _nu = _nu_prop[_qp];

  _c1 = _E*(1.-_nu)/(1.+_nu)/(1.-2.*_nu);
  _c2 = _nu/(1.-_nu);
  _c3 = .5*(1.-2.*_nu)/(1.-_nu);

  if (LIBMESH_DIM == 3)
  {
    
  _B11=TensorValue<Number>(1., 0., 0.,
                           0., _c3, 0.,
                           0., 0., _c3);

  _B12=TensorValue<Number>(0., _c2, 0.,
                           _c3, 0., 0.,
                           0., 0., 0.);

  _B13=TensorValue<Number>(0., 0., _c2,
                           0., 0., 0.,
                           _c3, 0., 0.);
  _B21=TensorValue<Number>(0., _c3, 0.,
                           _c2, 0., 0.,
                           0., 0., 0.);

  _B22=TensorValue<Number>(_c3, 0., 0.,
                           0., 1., 0.,
                           0., 0., _c3);

  _B23=TensorValue<Number>(0., 0., 0.,
                           0., 0., _c2,
                           0., _c3, 0.);
  _B31=TensorValue<Number>(0., 0., _c3,
                           0., 0., 0.,
                           _c2, 0., 0.);

  _B32=TensorValue<Number>(0., 0., 0.,
                           0., 0., _c3,
                           0., _c2, 0.);

  _B33=TensorValue<Number>(_c3, 0., 0.,
                           0., _c3, 0.,
                           0., 0., 1.);
  }
  
  else if (LIBMESH_DIM == 2)
  {

    _B11=TensorValue<Number>(1., 0.,
                             0., _c3);

    _B12=TensorValue<Number>(0., _c2,
                           _c3 , 0.0);

    _B21=TensorValue<Number>(0., _c3,
                           _c2 ,  0.);

    _B22=TensorValue<Number>(_c3, 0.,
                             0., 1.);

  }
  
}
