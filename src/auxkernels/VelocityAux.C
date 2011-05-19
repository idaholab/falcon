#include "VelocityAux.h"

template<>
InputParameters validParams<VelocityAux>()
{
     InputParameters params = validParams<AuxKernel>();
     params.addRequiredCoupledVar("pressure", "Use pressure here to calculate Pore Velocity");
     params.addRequiredCoupledVar("density_water", "Use density here to calculate Pore Velocity");
     //    params.addRequiredCoupledVar("porosity", "Use density here to calculate Pore Velocity");
     
     params.addParam<int>("i",0,"component of the pressure vector");
     return params;
}

VelocityAux::VelocityAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),

   _grad_p(coupledGradient("pressure")),
   _pressure(coupledValue("pressure")),
   _density_water(coupledValue("density_water")),
   // _porosity(coupledValue("porosity")),
   
   _tau_water(getMaterialProperty<Real>("tau_water")),
   _gravity(getMaterialProperty<Real>("gravity")),
   _gravity_vector(getMaterialProperty<RealVectorValue>("gravity_vector")),
   _porosity(getMaterialProperty<Real>("material_porosity")),
   // _density_water(getMaterialProperty<Real>("density_water")),
   _i(getParam<int>("i"))

{}

Real
VelocityAux::computeValue()
{

 if (_porosity[_qp] == 0.0) //then set velocity to 0, this avoids a divide by zero error.
 {
   return 0.0;
 }
       
 else  //calculate the pore velocity
 {
   return (-((_tau_water[_qp]*_grad_p[_qp](_i)) / (_density_water[_qp]* _porosity[_qp])))
        +
        (-((_tau_water[_qp]*_gravity[_qp]*_gravity_vector[_qp](_i)) / (_porosity[_qp])));
 }
}
