/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

//! Author:  Yidong Xia (Yidong.Xia@inl.gov)
//! Created: 09/09/2014

#include "SUPGOneD.h"

template<>
InputParameters validParams<SUPGOneD>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<bool>("discontinuity_capturing", false, "flag for discontinuity capturing, default = false");
  params.addParam<int>("method",0,"Choice for the discontinuity-capturing algorithm");
  params.addParam<Real>("alpha", 1.0, "Scaling Coefficient");
  params.addRequiredParam<RealVectorValue>("velocity", "Velocity Vector");
  return params;
}

SUPGOneD::SUPGOneD(const std::string & name,
                       InputParameters parameters) :
  Kernel(name, parameters),
  _u_old(valueOld()),
  _discontinuity_capturing(getParam<bool>("discontinuity_capturing")),
  _method(getParam<int>("method")),
  _alpha(getParam<Real>("alpha")),
  _velocity(getParam<RealVectorValue>("velocity"))
{}

Real SUPGOneD::computeQpResidual()
{
  Real term1 = 0.0;
  Real term2 = 0.0;

  const double hsize = _current_elem->volume();

  // Magnitude of the local convective velocity 
  Real vmod = std::sqrt( _velocity(0) * _velocity(0) +
                         _velocity(1) * _velocity(1) +
                         _velocity(2) * _velocity(2) );

  // Streamline stabilization parameter
  Real tau1 = hsize*hsize / (2.0*hsize*vmod);

  // Residual of the governing equation in strong form
  Real strong_residual = (_u[_qp] - _u_old[_qp])/_dt + _velocity*_grad_u[_qp];

  term1 = _alpha * tau1 * _velocity * _grad_test[_i][_qp] * strong_residual;

  // Discontinuity-capturing term: not validated yet (do not use!)
  if( _discontinuity_capturing ) {
    Real grad_u_mod2 = _grad_u[_qp](0) * _grad_u[_qp](0) +
                       _grad_u[_qp](1) * _grad_u[_qp](1) +
                       _grad_u[_qp](2) * _grad_u[_qp](2);

    Real grad_u_mod = std::sqrt(grad_u_mod2);

    Real delta;
    switch (_method) {
      // Hughes et al. 1986
      case 0:
        if(grad_u_mod > 1.0e-7) {
          Real tau2 = tau1;
          term2 = tau2 * 
                  _velocity * _grad_u[_qp] / grad_u_mod2 * 
                  _grad_u[_qp] * _grad_test[_i][_qp] * 
                  strong_residual;

        }
        break;
      // Beau et al. 1991
      case 1:
        delta = std::abs( hsize/2.0 * strong_residual / (grad_u_mod+1.0e-7) );
        term2 = delta * _grad_u[_qp] * _grad_test[_i][_qp];
        break;
      default:
        std::cout<<"Invalid parameter: "<<"_method = "<<_method<<std::endl;
        exit(0);
    }
  }
 
  return term1 + term2;
}

Real SUPGOneD::computeQpJacobian()
{
  Real term1 = 0.0;

  const double hsize = _current_elem->volume();

  // Magnitude of the local convective velocity 
  Real vmod = std::sqrt( _velocity(0) * _velocity(0) +
                         _velocity(1) * _velocity(1) +
                         _velocity(2) * _velocity(2) );

  // Streamline stabilization parameter
  Real tau1 = hsize*hsize / (2.0*hsize*vmod);

  // Jacobian of residual of the governing equation in strong form
  Real strong_residual = _phi[_j][_qp] / _dt + _velocity * _grad_phi[_j][_qp];

  term1 = _alpha * tau1 * _velocity * _grad_test[_i][_qp] * strong_residual;

  return term1;
}
