#ifndef COUPLEDDENSITYAUX_H
#define COUPLEDDENSITYAUX_H

#include "AuxKernel.h"


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
  CoupledDensityAux(std::string name, MooseSystem & moose_system, InputParameters parameters);

  virtual ~CoupledDensityAux() {}
  
protected:
  virtual Real computeValue();

  Real _value;

  Real _temperature;
  
  Real & _temperature_val;
};

#endif //COUPLEDDENSITYAUX_H
