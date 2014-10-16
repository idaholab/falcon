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

/** 
 * Authors: Yidong Xia (Yidong.Xia@inl.gov)
 * Created: 09/24/2014
 */

#ifndef DGCOUPLEDCONVECTION_H
#define DGCOUPLEDCONVECTION_H

#include "DGKernel.h"

class DGCoupledConvection;

template<>
InputParameters validParams<DGCoupledConvection>();


class DGCoupledConvection : public DGKernel
{
  public:

    DGCoupledConvection(const std::string & name, InputParameters parameters);

  protected:

    virtual Real computeQpResidual(Moose::DGResidualType type);
    virtual Real computeQpJacobian(Moose::DGJacobianType type);

  private:

    Real _alpha;

    VariableGradient & _grad_coupled;
    VariableGradient & _grad_neighbor_coupled;
};

#endif //DGCOUPLEDCONVECTION_H

