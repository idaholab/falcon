/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef PRESSURENEUMANNBC2_H
#define PRESSURENEUMANNBC2_H

#include "IntegratedBC.h"
#include "Material.h"


//Forward Declarations
class PressureNeumannBC2;

template<>
InputParameters validParams<PressureNeumannBC2>();

class PressureNeumannBC2 : public IntegratedBC
{
public:

  PressureNeumannBC2(const std::string & name, InputParameters parameters);

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

  const MaterialProperty<Real> & _gamma;
};

#endif //PRESSURENEUMANNBC2_H
