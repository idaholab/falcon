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
