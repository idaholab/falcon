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

#ifndef STOCHASTICFIELDAUX_H
#define STOCHASTICFIELDAUX_H

#include "AuxKernel.h"
#include "StochasticField.h"

class StochasticFieldAux : public AuxKernel
{
public:
  StochasticFieldAux(const InputParameters & parameters);
  virtual ~StochasticFieldAux() {};

protected:
  virtual Real computeValue();

private:
  StochasticField _stoch_field;

public:
  static InputParameters validParams();
};

#endif // STOCHASTICFIELDAUX_H
