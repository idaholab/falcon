#ifndef COUPLEDDENSITYAUX_H
#define COUPLEDDENSITYAUX_H

#include "AuxKernel.h"
//#include "CoupledDdensityDPAux.h"
//#include "CoupledDdensityDTAux.h"

//Forward Declarations
class CoupledDensityAux;

template<>
InputParameters validParams<CoupledDensityAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledDensityAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  CoupledDensityAux(const std::string & name, InputParameters parameters);

  virtual ~CoupledDensityAux() {}
  
protected:
    
   virtual Real computeValue();
   
   Real _input_density_mixture;
 
  
};

#endif //COUPLEDDENSITYAUX_H
