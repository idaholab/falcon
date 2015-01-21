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

#ifndef FLUX_H
#define FLUX_H

#include "IntegratedBC.h"

//Forward Declarations
class Flux;

template<>
InputParameters validParams<Flux>();

class Flux : public IntegratedBC
{
public:

  Flux(const std::string & name, InputParameters parameters);

  virtual ~Flux(){}

protected:
  virtual Real computeQpResidual(){return 0;}
  virtual Real computeQpIntegral();

private:
  Real _coefficient;
};

#endif //FLUXBC_H

