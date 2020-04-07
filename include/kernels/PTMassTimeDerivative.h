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

#ifndef PTMASSTIMEDERIVATIVE_H
#define PTMASSTIMEDERIVATIVE_H

class PTMassTimeDerivative : public TimeDerivative
{
  public:

    PTMassTimeDerivative(const InputParameters & parameters);

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

    const MaterialProperty<Real> & _poro;
    const MaterialProperty<Real> & _wrho;
    const MaterialProperty<Real> & _drop;

  public:
    static InputParameters validParams();
};
#endif //PTMASSTIMEDERIVATIVE_H
