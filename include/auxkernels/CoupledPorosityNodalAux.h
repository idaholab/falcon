#ifndef COUPLEDPOROSITYNODALAUX_H
#define COUPLEDPOROSITYNODALAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledPorosityNodalAux;

template<>
InputParameters validParams<CoupledPorosityNodalAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledPorosityNodalAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  CoupledPorosityNodalAux(const std::string & name, InputParameters parameters);

  virtual ~CoupledPorosityNodalAux() {}
  
protected:
  virtual Real computeValue();

//  VariableValue & _temperature;
  
  Real _input_porosity;
  
//  bool _has_variable_viscosity;


};

#endif //COUPLEDPOROSITYNODALAUX_H
