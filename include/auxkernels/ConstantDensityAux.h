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

#ifndef CONSTANTDENSITYAUX
#define CONSTANTDENSITYAUX

#include "AuxKernel.h"


//Forward Declarations
class ConstantDensityAux;

template<>
InputParameters validParams<ConstantDensityAux>();

/** 
 * Coupled auxiliary value
 */
class ConstantDensityAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  ConstantDensityAux(const std::string & name, InputParameters parameters);

  virtual ~ConstantDensityAux() {}
  
protected:
  virtual Real computeValue();
  Real _input_density_water;

};

#endif //CONSTANTDENSITYAUX
