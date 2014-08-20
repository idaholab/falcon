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
//! Created: 08/20/2014

#ifndef CONVECTION_H
#define CONVECTION_H

#include "Kernel.h"

class Convection;

template<>
InputParameters validParams<Convection>();

class Convection : public Kernel
{
  public:

    Convection(const std::string & name,
               InputParameters parameters);

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

  private:

    /**
     * Constant convection velocity vector
     */
    RealVectorValue _velocity;
};

#endif //CONVECTION_H
