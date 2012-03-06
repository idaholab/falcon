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

#ifndef APERTUREAUX_H
#define APERTUREAUX_H

#include "AuxKernel.h"


//Forward Declarations
class ApertureAux;

template<>
InputParameters validParams<ApertureAux>();

/** 
 * Coupled auxiliary value
 */
class ApertureAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all
   * derived classes can be built using the same
   * constructor.
   */
  ApertureAux(const std::string & name, InputParameters parameters);

  virtual ~ApertureAux() {}
  
protected:
  virtual Real computeValue();

  MaterialProperty<Real> & _aperture;

};

#endif //APERTUREAUX_H
