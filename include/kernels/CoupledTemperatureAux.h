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
  CoupledTemperatureAux(const std::string & name, MooseSystem & moose_system, InputParameters parameters);

  virtual ~CoupledTemperatureAux() {}
  
protected:
  virtual Real computeValue();

  Real E3;
  Real E5;
  Real E6;
  
    
  VariableValue & _enthalpy;
  VariableValue & _pressure;
  MaterialProperty<Real> & _Hw;
  MaterialProperty<Real> & _Hs;
};

#endif //COUPLEDTEMPERATUREAUX_H
