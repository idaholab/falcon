#ifndef COUPLEDVISCOSITYAUX_H
#define COUPLEDVISCOSITYAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledViscosityAux;

template<>
InputParameters validParams<CoupledViscosityAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledViscosityAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  CoupledViscosityAux(const std::string & name, InputParameters parameters);

  virtual ~CoupledViscosityAux() {}
  
protected:
  virtual Real computeValue();

  Real _temperature;
  
  VariableValue & _temperature_val;

  Real _a;
  Real _b;
  Real _c;


  Real _mu_w;
};

#endif //COUPLEDVISCOSITYAUX_H
