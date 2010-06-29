#ifndef COUPLEDTEMPERATUREAUX_H
#define COUPLEDTEMPERATUREAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledTemperatureAux;

template<>
InputParameters validParams<CoupledTemperatureAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledTemperatureAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  CoupledTemperatureAux(std::string name, MooseSystem & moose_system, InputParameters parameters);

  virtual ~CoupledTemperatureAux() {}
  
protected:
  virtual Real computeValue();

    
  MooseArray<Real> & _enthalpy;
  MooseArray<Real> & _pressure;
};

#endif //COUPLEDTEMPERATUREAUX_H
