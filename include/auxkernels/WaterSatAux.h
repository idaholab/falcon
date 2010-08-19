#ifndef WATERSATAUX_H
#define WATERSATAUX_H

#include "AuxKernel.h"


//Forward Declarations
class WaterSatAux;

template<>
InputParameters validParams<WaterSatAux>();

/** 
 * Coupled auxiliary value
 */
class WaterSatAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  WaterSatAux(std::string name, MooseSystem & moose_system, InputParameters parameters);

  virtual ~WaterSatAux() {}
  
protected:
  virtual Real computeValue();

    
  MaterialProperty<Real> & _sat_w;
  VariableValue & _pressure;
  VariableValue & _enthalpy;

  
     Real E3;
     Real E6;
     Real E7;

// coefficients related to rho_w empirical equation     
     Real a1;
     Real a2;
     Real a3;
     Real a4;
     Real a5;
     Real a6;

// coefficients related to rho_s empirical equation
     Real b1;
     Real b2;
     Real b3;
     Real b4;
     Real b5;

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
  
};

#endif //WATERSATAUX_H
