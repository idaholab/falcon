#include "Material.h"
#include "HuyakornMassConvection.h"

template<>
InputParameters validParams<HuyakornMassConvection>()
{
  InputParameters params = validParams<Diffusion>();
  return params;
}

HuyakornMassConvection::HuyakornMassConvection(const std::string & name,
                                             InputParameters parameters)
  :Diffusion(name, parameters),
    _tau(getMaterialProperty<Real>("tau"))
{}

Real
HuyakornMassConvection::computeQpResidual()
{
  return _tau[_qp]*Diffusion::computeQpResidual();
}

Real
HuyakornMassConvection::computeQpJacobian()
{
  return _tau[_qp]*Diffusion::computeQpJacobian();
}
