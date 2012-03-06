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

#ifndef COUPLEDDDENSITYDTAUX_H_PT
#define COUPLEDDDENSITYDTAUX_H_PT

#include "AuxKernel.h"


//Forward Declarations
class CoupledDdensityDTAux_PT;

template<>
InputParameters validParams<CoupledDdensityDTAux_PT>();

/** 
 * Coupled auxiliary value
 */
class CoupledDdensityDTAux_PT : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  CoupledDdensityDTAux_PT(const std::string & name, InputParameters parameters);

  virtual ~CoupledDdensityDTAux_PT() {}
  
protected:
  virtual Real computeValue();

  // Real density_fun(Real T);

  VariableValue & _temperature;
  VariableValue & _pressure;
  
//  Real _input_density_water;
//  bool _has_variable_density;
//  Real _density_subroutine_val;
  // Real _density_water;
  
  

  
};

#endif //COUPLEDDDENSITYDTAUX_H_PT
