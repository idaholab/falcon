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

#ifndef SOLIDMECHPOROCOUPLE_H
#define SOLIDMECHPOROCOUPLE_H

#include "SolidMechFalcon.h"

//libMesh includes
#include "libmesh/tensor_value.h"


//Forward Declarations
class SolidMechPoroCouple;

template<>
InputParameters validParams<SolidMechPoroCouple>();

class SolidMechPoroCouple : public SolidMechFalcon
{
public:

  SolidMechPoroCouple(const std::string & name, InputParameters parameters);
  
  void recomputeCouplingConstants();
  
protected:
  unsigned int _pressure_var;
  MaterialProperty<Real> & _biot_coeff;
  VariableGradient & _grad_pressure;
  VariableValue  & _pressure_val;

};
 

#endif //SOLIDMECHPOROCOUPLE_H
