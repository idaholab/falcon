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
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  VelocityAux(const std::string & name, MooseSystem & moose_system, InputParameters parameters);

  virtual ~VelocityAux() {}
  
protected:
  virtual Real computeValue();

   int _i;
  MaterialProperty<RealGradient> &_pore_velocity_w; 
};

#endif //VELOCITYAUX_H
