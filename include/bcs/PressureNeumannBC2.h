#ifndef PRESSURENEUMANNBC2_H
#define PRESSURENEUMANNBC2_H

#include "BoundaryCondition.h"
#include "Material.h"


//Forward Declarations
class PressureNeumannBC2;

template<>
InputParameters validParams<PressureNeumannBC2>();

class PressureNeumannBC2 : public BoundaryCondition
{
public:

  PressureNeumannBC2(std::string name, MooseSystem & moose_system, InputParameters parameters);

  virtual ~PressureNeumannBC2(){}

protected:

  /**
   * This is here because materials don't yet work on boundaries!
   */
  Real pressure();
  
  virtual Real computeQpResidual();
  
  unsigned int _pe_var;
  VariableValue & _pe;

  unsigned int _u_vel_var;
  VariableValue & _u_vel;

  unsigned int _v_vel_var;
  VariableValue & _v_vel;

  unsigned int _w_vel_var;
  VariableValue & _w_vel;

  int _component;

  MaterialProperty<Real> & _gamma;
};

#endif //PRESSURENEUMANNBC2_H
