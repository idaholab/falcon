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

#include "TimeDerivative.h"

#ifndef PTMASSTIMEDERIVATIVE_H
#define PTMASSTIMEDERIVATIVE_H

//Forward Declarations
class PTMassTimeDerivative;

template<>
InputParameters validParams<PTMassTimeDerivative>();

class PTMassTimeDerivative : public TimeDerivative
{
  public:

    PTMassTimeDerivative(const std::string & name, InputParameters parameters);

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

    MaterialProperty<Real> & _poro;
    MaterialProperty<Real> & _wrho;
    MaterialProperty<Real> & _drop;
};
#endif //PTMASSTIMEDERIVATIVE_H
