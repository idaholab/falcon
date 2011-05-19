#ifndef VELOCITYAUX_H
#define VELOCITYAUX_H

#include "AuxKernel.h"


//Forward Declarations
class VelocityAux;

template<>
InputParameters validParams<VelocityAux>();

/** 
 * Coupled auxiliary value
 */
class VelocityAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all
   * derived classes can be built using the same
   * constructor.
   */
  VelocityAux(const std::string & name, InputParameters parameters);

  virtual ~VelocityAux() {}
  
protected:
  virtual Real computeValue();

  VariableGradient & _grad_p;
  VariableValue  & _pressure;
  VariableValue  & _density_water;
//  VariableValue  & _porosity;

  MaterialProperty<Real> & _tau_water;
  MaterialProperty<Real> & _gravity;
  MaterialProperty<RealVectorValue> & _gravity_vector;
  MaterialProperty<Real> & _porosity;
//  MaterialProperty<Real> & _density_water;
  int _i;

};

#endif //VELOCITYAUX_H
