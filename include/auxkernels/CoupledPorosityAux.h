#ifndef COUPLEDPOROSITYAUX_H
#define COUPLEDPOROSITYAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledPorosityAux;

template<>
InputParameters validParams<CoupledPorosityAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledPorosityAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  CoupledPorosityAux(const std::string & name, InputParameters parameters);

  virtual ~CoupledPorosityAux() {}
  
protected:
  virtual Real computeValue();

//  VariableValue & _temperature;
  Real _input_porosity;
//  bool _has_variable_viscosity;


};

#endif //COUPLEDPOROSITYAUX_H
