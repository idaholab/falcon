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

#ifndef ANALYTICALADE1D_H
#define ANALYTICALADE1D_H

#include "AuxKernel.h"


//Forward Declarations
class AnalyticalADE1D;

template<>
InputParameters validParams<AnalyticalADE1D>();

/** 
 * Coupled auxiliary value
 */
class AnalyticalADE1D : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  AnalyticalADE1D(const std::string & name, InputParameters parameters);

  virtual ~AnalyticalADE1D() {}
  
protected:
  virtual Real computeValue();
};

#endif //ANALYTICALADE1D_H
