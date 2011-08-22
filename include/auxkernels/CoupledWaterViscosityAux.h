#ifndef COUPLEDWATERVISCOSITYAUX_H
#define COUPLEDWATERVISCOSITYAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledWaterViscosityAux;

template<>
InputParameters validParams<CoupledWaterViscosityAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledWaterViscosityAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  CoupledWaterViscosityAux(const std::string & name, InputParameters parameters);

  virtual ~CoupledWaterViscosityAux() {}
  
protected:
  virtual Real computeValue();

  // Real viscosity_fun(Real T);
  
  VariableValue & _density_water;
  VariableValue & _temperature;
  
  Real _input_viscosity_water;
  bool _has_variable_viscosity;
  
};

#endif //COUPLEDVISCOSITYAUX_H
