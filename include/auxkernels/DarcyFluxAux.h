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

#ifndef DARCYFLUXAUX_H
#define DARCYFLUXAUX_H

#include "AuxKernel.h"


//Forward Declarations
class DarcyFluxAux;

template<>
InputParameters validParams<DarcyFluxAux>();

/**
 * Coupled auxiliary value
 */
class DarcyFluxAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all
   * derived classes can be built using the same
   * constructor.
   */
  DarcyFluxAux(const std::string & name, InputParameters parameters);

  virtual ~DarcyFluxAux() {}

protected:
  virtual Real computeValue();

  const MaterialProperty<RealGradient> & _darcy_flux_water;
  const MaterialProperty<RealGradient> & _darcy_flux_steam;
  MooseEnum _phase;
  int _i;

};

#endif //DARCYFLUXAUX_H
