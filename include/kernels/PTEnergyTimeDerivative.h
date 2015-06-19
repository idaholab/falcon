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

#ifndef PTENERGYTIMEDERIVATIVE_H
#define PTENERGYTIMEDERIVATIVE_H

class PTEnergyTimeDerivative;

template<>
InputParameters validParams<PTEnergyTimeDerivative>();

class PTEnergyTimeDerivative : public TimeDerivative
{
  public:

    PTEnergyTimeDerivative(const std::string & name, InputParameters parameters);

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

    MaterialProperty<Real> & _epor;
};
#endif //PTENERGYTIMEDERIVATIVE_H
