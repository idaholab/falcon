#ifndef COUPLEDPOROSITYMATERIALAUX_H
#define COUPLEDPOROSITYMATERIALAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledPorosityMaterialAux;

template<>
InputParameters validParams<CoupledPorosityMaterialAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledPorosityMaterialAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  CoupledPorosityMaterialAux(const std::string & name, InputParameters parameters);

  virtual ~CoupledPorosityMaterialAux() {}
  
protected:
  virtual Real computeValue();

//  VariableValue & _temperature;
  
  MaterialProperty<Real> & _input_porosity;
  
//  bool _has_variable_viscosity;


};

#endif //COUPLEDPOROSITYMATERIALAUX_H
