#ifndef COUPLEDEOSAUX_H
#define COUPLEDEOSAUX_H

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

  // Real density_fun(Real T);

  VariableValue & _temperature;
  VariableValue & _pressure;
  VariableValue & _dwdt;
  VariableValue & _dwdp;
    
  Real _input_density_water;
  bool _has_variable_density;
  Real _density_subroutine_val;
  // Real _density_water;
  
  

  
};

#endif //COUPLEDDENSITYAUX_H
