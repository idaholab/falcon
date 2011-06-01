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
