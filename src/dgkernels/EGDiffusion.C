#include "EGDiffusion.h"

// MOOSE includes
#include "MooseVariableFE.h"

#include "libmesh/utility.h"

registerMooseObject("FalconApp", EGDiffusion);

InputParameters
EGDiffusion::validParams()
{
  InputParameters params = DGKernel::validParams();
  params.addClassDescription("Computes residual contribution for the diffusion operator using "
                             "discontinous Galerkin method.");
  // See header file for sigma and epsilon
  params.addParam<MaterialPropertyName>("sigma", "sigma");
  params.addRequiredParam<Real>("epsilon", "epsilon");
  params.addParam<MaterialPropertyName>(
      "diff", 1., "The diffusion (or thermal conductivity or viscosity) coefficient.");
  params.addRequiredCoupledVar("v", "The governing variable that controls diffusion of u.");
  return params;
}

EGDiffusion::EGDiffusion(const InputParameters & parameters)
  : DGKernel(parameters),
    _epsilon(getParam<Real>("epsilon")),
    _sigma(getMaterialProperty<Real>("sigma")),
    _sigma_neighbor(getNeighborMaterialProperty<Real>("sigma")),
    _diff(getMaterialProperty<Real>("diff")),
    _diff_neighbor(getNeighborMaterialProperty<Real>("diff")),
    _v_var(dynamic_cast<MooseVariable &>(*getVar("v", 0))),
    _v(_v_var.sln()),
    _v_neighbor(_v_var.slnNeighbor()),
    _grad_v(_v_var.gradSln()),
    _grad_v_neighbor(_v_var.gradSlnNeighbor()),
    _v_id(coupled("v"))
{
}

// This kernel is based on the DG Diffusion Kernel and It is recommended to rewrite the averging
// through the interface for the diffusion coeff to be harmonic not arithmetic average.

Real
EGDiffusion::computeQpResidual(Moose::DGResidualType type)
{
  Real r = 0;

  const unsigned int elem_b_order = _var.order();
  Real h_elem = _current_elem->hmin();
  if (elem_b_order != 0)
  {
    h_elem = _current_elem_volume / _current_side_volume * 1. / Utility::pow<2>(elem_b_order);
  }

  switch (type)
  {
    case Moose::Element:
      r -= 0.5 *
           (_diff[_qp] * _grad_v[_qp] * _normals[_qp] +
            _diff_neighbor[_qp] * _grad_v_neighbor[_qp] * _normals[_qp]) *
           _test[_i][_qp];
      r += _epsilon * 0.5 * (_v[_qp] - _v_neighbor[_qp]) * _diff[_qp] * _grad_test[_i][_qp] *
           _normals[_qp];
      r += _sigma[_qp] * _diff[_qp] / h_elem * (_v[_qp] - _v_neighbor[_qp]) * _test[_i][_qp];
      break;

    case Moose::Neighbor:
      r += 0.5 *
           (_diff[_qp] * _grad_v[_qp] * _normals[_qp] +
            _diff_neighbor[_qp] * _grad_v_neighbor[_qp] * _normals[_qp]) *
           _test_neighbor[_i][_qp];
      r += _epsilon * 0.5 * (_v[_qp] - _v_neighbor[_qp]) * _diff_neighbor[_qp] *
           _grad_test_neighbor[_i][_qp] * _normals[_qp];
      r -= _sigma_neighbor[_qp] * _diff_neighbor[_qp] / h_elem * (_v[_qp] - _v_neighbor[_qp]) *
           _test_neighbor[_i][_qp];
      break;
  }

  return r;
}
Real
EGDiffusion::computeQpJacobian(Moose::DGJacobianType /*type*/)
{
  return 0;
}
Real
EGDiffusion::computeQpOffDiagJacobian(Moose::DGJacobianType type, unsigned int jvar)
{
  if (jvar != _v_id)
    return 0;

  Real r = 0;

  const unsigned int elem_b_order = _var.order();
  Real h_elem = _current_elem->hmin();
  if (elem_b_order != 0)
  {
    h_elem = _current_elem_volume / _current_side_volume * 1. / Utility::pow<2>(elem_b_order);
  }

  if (jvar == _v_id)
  {
    switch (type)
    {
      case Moose::ElementElement:
        r -= 0.5 * _diff[_qp] * _grad_phi[_j][_qp] * _normals[_qp] * _test[_i][_qp];
        r += _epsilon * 0.5 * _phi[_j][_qp] * _diff[_qp] * _grad_test[_i][_qp] * _normals[_qp];
        r += _sigma[_qp] * _diff[_qp] / h_elem * _phi[_j][_qp] * _test[_i][_qp];
        break;

      case Moose::ElementNeighbor:
        r -= 0.5 * _diff_neighbor[_qp] * _grad_phi_neighbor[_j][_qp] * _normals[_qp] *
             _test[_i][_qp];
        r += _epsilon * 0.5 * -_phi_neighbor[_j][_qp] * _diff[_qp] * _grad_test[_i][_qp] *
             _normals[_qp];
        r += _sigma[_qp] * _diff[_qp] / h_elem * -_phi_neighbor[_j][_qp] * _test[_i][_qp];
        break;

      case Moose::NeighborElement:
        r += 0.5 * _diff[_qp] * _grad_phi[_j][_qp] * _normals[_qp] * _test_neighbor[_i][_qp];
        r += _epsilon * 0.5 * _phi[_j][_qp] * _diff_neighbor[_qp] * _grad_test_neighbor[_i][_qp] *
             _normals[_qp];
        r -= _sigma_neighbor[_qp] * _diff_neighbor[_qp] / h_elem * _phi[_j][_qp] *
             _test_neighbor[_i][_qp];
        break;

      case Moose::NeighborNeighbor:
        r += 0.5 * _diff_neighbor[_qp] * _grad_phi_neighbor[_j][_qp] * _normals[_qp] *
             _test_neighbor[_i][_qp];
        r += _epsilon * 0.5 * -_phi_neighbor[_j][_qp] * _diff_neighbor[_qp] *
             _grad_test_neighbor[_i][_qp] * _normals[_qp];
        r -= _sigma_neighbor[_qp] * _diff_neighbor[_qp] / h_elem * -_phi_neighbor[_j][_qp] *
             _test_neighbor[_i][_qp];
        break;
    }
  }

  return r;
}
