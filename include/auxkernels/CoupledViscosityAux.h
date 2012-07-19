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
  
MaterialProperty<Real> & _viscosity_water;
  // Real viscosity_fun(Real T);
  
  //VariableValue & _temperature;
  //VariableValue & _pressure;
  
  //Real _viscosity_water;
  //bool _has_variable_viscosity;
  //Real _viscosity_subroutine_val;

};

#endif //COUPLEDVISCOSITYAUX_H
