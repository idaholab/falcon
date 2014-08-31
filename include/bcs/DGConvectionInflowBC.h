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
//! Created: 08/27/2014

#ifndef DGCONVECTIONINFLOWBC_H
#define DGCONVECTIONINFLOWBC_H

#include "IntegratedBC.h"

//Forward Declarations
class DGConvectionInflowBC;

template<>
InputParameters validParams<DGConvectionInflowBC>();

class DGConvectionInflowBC : public IntegratedBC
{
  public:

    DGConvectionInflowBC(const std::string & name, InputParameters parameters);

    virtual ~DGConvectionInflowBC() {}

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

  private:

    /**
     * Variable value at inflow
     */
    Real _value;

    /**
     * Constant convection velocity vector
     */
    RealVectorValue _velocity;
};

#endif //DGCONVECTIONINFLOWBC_H
