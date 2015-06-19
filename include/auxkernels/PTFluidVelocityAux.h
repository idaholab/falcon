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

#ifndef PTFLUIDVELOCITYAUX_H
#define PTFLUIDVELOCITYAUX_H

#include "PTDarcyFluxAux.h"

class PTFluidVelocityAux;

template<>
InputParameters validParams<PTFluidVelocityAux>();

class PTFluidVelocityAux : public PTDarcyFluxAux
{
  public:

    PTFluidVelocityAux(const std::string & name, InputParameters parameters);

    virtual ~PTFluidVelocityAux() {}

  protected:

    virtual Real computeValue();

    MaterialProperty<Real> & _poro;
};
#endif //PTFLUIDVELOCITYAUX_H
