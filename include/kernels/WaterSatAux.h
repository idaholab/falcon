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
  
};

#endif //WATERSATAUX_H
