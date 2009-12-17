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
  CoupledViscosityAux(std::string name,
            InputParameters parameters,
            std::string var_name,
            std::vector<std::string> coupled_to,
            std::vector<std::string> coupled_as);

  virtual ~CoupledViscosityAux() {}
  
protected:
  virtual Real computeValue();

  Real _a;
  Real _b;
  Real _c;


  Real _mu_w;

  Real _temperature;
  
  Real & _temperature_val;
};

#endif //COUPLEDVISCOSITYAUX_H
