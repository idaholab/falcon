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

#ifndef COUPLEDSTEAMDENSITYAUX_H
#define COUPLEDSTEAMDENSITYAUX_H

#include "AuxKernel.h"
//#include "CoupledDdensityDPAux.h"
//#include "CoupledDdensityDTAux.h"

//Forward Declarations
class CoupledSteamDensityAux;

template<>
InputParameters validParams<CoupledSteamDensityAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledSteamDensityAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  CoupledSteamDensityAux(const std::string & name, InputParameters parameters);

  virtual ~CoupledSteamDensityAux() {}
  
protected:
   virtual Real computeValue();
   
   Real _input_density_steam;
 
  
};

#endif //COUPLEDDENSITYAUX_H
