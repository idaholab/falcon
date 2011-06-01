#ifndef COUPLEDDDENSITYDTAUX_H
#define COUPLEDDDENSITYDTAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledDdensityDTAux;

template<>
InputParameters validParams<CoupledDdensityDTAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledDdensityDTAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  CoupledDdensityDTAux(const std::string & name, InputParameters parameters);

  virtual ~CoupledDdensityDTAux() {}
  
protected:
  virtual Real computeValue();

  
};

#endif //COUPLEDDDENSITYDTAUX_H
