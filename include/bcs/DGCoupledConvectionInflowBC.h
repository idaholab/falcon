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

#ifndef DGCOUPLEDCONVECTIONINFLOWBC_H
#define DGCOUPLEDCONVECTIONINFLOWBC_H

#include "IntegratedBC.h"

//Forward Declarations
class DGCoupledConvectionInflowBC;

template<>
InputParameters validParams<DGCoupledConvectionInflowBC>();

class DGCoupledConvectionInflowBC : public IntegratedBC
{
  public:

    DGCoupledConvectionInflowBC(const std::string & name, InputParameters parameters);

    virtual ~DGCoupledConvectionInflowBC() {}

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

  private:

    Real _value;

    VariableGradient & _grad_coupled;
};

#endif //DGCOUPLEDCONVECTIONINFLOWBC_H
