/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c)     2012 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

//! Authors: Yidong Xia (Yidong.Xia@inl.gov)
//! Created: 09/07/2014

#ifndef VARIABLEGRADIENTAUX_H
#define VARIABLEGRADIENTAUX_H

#include "AuxKernel.h"

/**
 * Coupled auxiliary value
 */
class VariableGradientAux : public AuxKernel
{
  public:

    /**
     * Factory constructor, takes parameters so that all
     * derived classes can be built using the same
     * constructor.
     */
    VariableGradientAux(const InputParameters & parameters);

    virtual ~VariableGradientAux() {}

  protected:
    virtual Real computeValue();

    const VariableGradient & _grad_coupled_variable;

    int _i;

  public:
    static InputParameters validParams();
};

#endif //VARIABLEGRADIENTAUX_H
