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

#ifndef FRACTIPLOCATIONAUX_H
#define FRACTIPLOCATIONAUX_H

#include "AuxKernel.h"
#include "libmesh/plane.h"
#include "libmesh/point.h"
#include "MooseEnum.h"
#include <string>

//Forward Declarations
class FracTipLocationAux;

template<>
InputParameters validParams<FracTipLocationAux>();

/** 
 * Coupled auxiliary value
 */
class FracTipLocationAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all
   * derived classes can be built using the same
   * constructor.
   */
  FracTipLocationAux(const std::string & name, InputParameters parameters);

  virtual ~FracTipLocationAux() {}
  
protected:
  virtual Real computeValue();
  
  VariableValue & _fracture_map;
  std::vector<Real> _wellbore_location;
      
  

};

#endif //FRACTIPLOCATIONAUX_H
