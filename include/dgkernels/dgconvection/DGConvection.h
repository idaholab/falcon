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
//! Created: 08/19/2014

#ifndef DGCONVECTION_H
#define DGCONVECTION_H

#include "DGKernel.h"

//Forward Declarations
class DGConvection;

template<>
InputParameters validParams<DGConvection>();


class DGConvection : public DGKernel
{
  public:
    DGConvection(const std::string & name, InputParameters parameters);

  protected:
    virtual Real computeQpResidual(Moose::DGResidualType type);
    virtual Real computeQpJacobian(Moose::DGJacobianType type);
    /**
     * A scaling factor for spectral radius
     */
    Real _alpha;
    /**
     * Constant convection velocity vector
     */
    RealVectorValue _velocity;
};

#endif //DGCONVECTION_H

