#include "MassFluxTimeDerivative.h"
#include "Material.h"

template<>
InputParameters validParams<MassFluxTimeDerivative>()
{
  InputParameters params = validParams<TimeDerivative>();
  return params;
}

MassFluxTimeDerivative::MassFluxTimeDerivative(const std::string & name, InputParameters parameters)
  :TimeDerivative(name, parameters),


   _density_water(getMaterialProperty<Real>("density_water")),
   _compressibility(getMaterialProperty<Real>("compressibility")),
   _porosity(getMaterialProperty<Real>("porosity"))
   
{}

Real
MassFluxTimeDerivative::computeQpResidual()
{

    return _compressibility[_qp]*_porosity[_qp]*_density_water[_qp]*TimeDerivative::computeQpResidual();
  
}

Real
MassFluxTimeDerivative::computeQpJacobian()
{    

    return _compressibility[_qp]*_porosity[_qp]*_density_water[_qp]*TimeDerivative::computeQpJacobian();
  
}
