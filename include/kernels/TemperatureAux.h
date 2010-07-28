#ifndef TEMPERATUREAUX_H
#define TEMPERATUREAUX_H

#include "AuxKernel.h"


//Forward Declarations
class TemperatureAux;

template<>
InputParameters validParams<TemperatureAux>();

/** 
 * Coupled auxiliary value
 */
class TemperatureAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  TemperatureAux(std::string name, MooseSystem & moose_system, InputParameters parameters);

  virtual ~TemperatureAux() {}
  
protected:
  virtual Real computeValue();

  MaterialProperty<Real> & _temp;
  
};

#endif //TEMPERATUREAUX_H
