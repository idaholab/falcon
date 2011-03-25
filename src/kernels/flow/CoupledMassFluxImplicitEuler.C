#include "CoupledMassFluxImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<CoupledMassFluxImplicitEuler>()
{
     InputParameters params = validParams<ImplicitEuler>();
     return params;
}

CoupledMassFluxImplicitEuler::CoupledMassFluxImplicitEuler(const std::string & name, InputParameters parameters)
  :ImplicitEuler(name, parameters),


   _density_water(getMaterialProperty<Real>("density_water")),
   _compressibility(getMaterialProperty<Real>("compressibility")),
   _porosity(getMaterialProperty<Real>("porosity"))
   
{}

Real
CoupledMassFluxImplicitEuler::computeQpResidual()
{

    return _compressibility[_qp]*_porosity[_qp]*_density_water[_qp]*ImplicitEuler::computeQpResidual();
  
}

Real
CoupledMassFluxImplicitEuler::computeQpJacobian()
{    

    return _compressibility[_qp]*_porosity[_qp]*_density_water[_qp]*ImplicitEuler::computeQpJacobian();
  
}
