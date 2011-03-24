#include "CoupledMassFluxImplicitEuler.h"
#include "Material.h"

template<>
InputParameters validParams<CoupledMassFluxImplicitEuler>()
{
     InputParameters params = validParams<ImplicitEuler>();

//RKP Comment this out for now, just use material property     
//     params.addCoupledVar("density", "TODO: add description");
//     params.addCoupledVar("compressibility", "TODO: add description");
     
     return params;
}

CoupledMassFluxImplicitEuler::CoupledMassFluxImplicitEuler(const std::string & name, InputParameters parameters)
  :ImplicitEuler(name, parameters),

//RKP comment this out for now until build coupled EOS   
//   _compressibility(coupledValue("compressibility")),
//   _density(coupledValue("density")),
 
//Use these instead
   _density_water(getMaterialProperty<Real>("density_water")),
   _compressibility(getMaterialProperty<Real>("compressibility")),

   
   _porosity(getMaterialProperty<Real>("porosity"))
   
{}

Real
CoupledMassFluxImplicitEuler::computeQpResidual()
{
//  return _compressibility[_qp]*_porosity[_qp]*_density[_qp]*ImplicitEuler::computeQpResidual();
    return _compressibility[_qp]*_porosity[_qp]*_density_water[_qp]*ImplicitEuler::computeQpResidual();
  
}

Real
CoupledMassFluxImplicitEuler::computeQpJacobian()
{    
//  return _compressibility[_qp]*_porosity[_qp]*_density[_qp]*ImplicitEuler::computeQpJacobian();
    return _compressibility[_qp]*_porosity[_qp]*_density_water[_qp]*ImplicitEuler::computeQpJacobian();
  
}
