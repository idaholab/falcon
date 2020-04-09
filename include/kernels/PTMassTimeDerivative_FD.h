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

#include "TimeDerivative.h"

#ifndef PTMASSTIMEDERIVATIVE_FD_H
#define PTMASSTIMEDERIVATIVE_FD_H

class PTMassTimeDerivative_FD : public TimeDerivative
{
  public:

    PTMassTimeDerivative_FD(const InputParameters & parameters);

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

    const MaterialProperty<Real> & _poro;
    const MaterialProperty<Real> & _wrho;
    const MaterialProperty<Real> & _wrho_old;
    const MaterialProperty<Real> & _drop;

  public:
    static InputParameters validParams();
};
#endif
