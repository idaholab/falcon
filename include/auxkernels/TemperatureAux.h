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
  VariableValue & _pressure;
  VariableValue & _enthalpy;

     Real E3;
     Real E6;
     Real E7;
// coefficients related to Hs empirical equation
     Real c1;
     Real c2;
     Real c3;
     Real c4;
     
// coefficients related to Hw empirical equation
     Real d1;
     Real d2;
     Real d3;
     Real d4;
     Real d5;
     Real d6;
     Real d7;

// coefficients related to Tw empirical equation
     Real w1;
     Real w2;
     Real w3;
     Real w4;
     Real w5;
     
// coefficients related to Ts empirical equation
     Real s1;
     Real s2;
     Real s3;
     Real s4;
     Real s5;
     Real s6;
     Real s7;
     Real s8;

};

#endif //TEMPERATUREAUX_H
