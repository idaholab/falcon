#ifndef COUPLEDEOSAUX_H_PT
#define COUPLEDEOSAUX_H_PT

#include "AuxKernel.h"
//#include "CoupledDdensityDPAux.h"
//#include "CoupledDdensityDTAux.h"

//Forward Declarations
class CoupledDensityAux_PT;

template<>
InputParameters validParams<CoupledDensityAux_PT>();

/** 
 * Coupled auxiliary value
 */
class CoupledDensityAux_PT : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  CoupledDensityAux_PT(const std::string & name, InputParameters parameters);

  virtual ~CoupledDensityAux_PT() {}
  
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

#endif //COUPLEDDENSITYAUX_H_PT
