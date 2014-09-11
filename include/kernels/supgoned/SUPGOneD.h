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

#ifndef SUPGONED_H
#define SUPGONED_H

#include "Kernel.h"

class SUPGOneD;

template<>
InputParameters validParams<SUPGOneD>();

class SUPGOneD : public Kernel
{
  public:

    SUPGOneD(const std::string & name, InputParameters parameters);

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

    VariableValue & _u_old;

    /**
     * Flag for discontinuity capturing
     */
    bool _discontinuity_capturing;

    /**
     * Choice for the discontinuity-capturing algorithm
     */
    int _method;

    /**
     * A scaling coefficient
     */
    Real _alpha;

    /**
     * Constant convection velocity vector
     */
    RealVectorValue _velocity;
};

#endif //SUPGONED_H
