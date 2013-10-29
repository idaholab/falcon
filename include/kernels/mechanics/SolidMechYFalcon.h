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

#ifndef SOLIDMECHYFALCON
#define SOLIDMECHYFALCON

#include "SolidMechFalcon.h"


//Forward Declarations
class SolidMechYFalcon;

template<>
InputParameters validParams<SolidMechYFalcon>();

class SolidMechYFalcon : public SolidMechFalcon
{
public:

  SolidMechYFalcon(const std::string & name, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  
private:
  const unsigned int _mesh_dimension;
  
  unsigned int _x_var;
  VariableValue  & _x;
  VariableGradient & _grad_x;
  
  unsigned int _z_var;
  VariableValue  & _z;
  VariableGradient & _grad_z;
};
#endif //SOLIDMECHYFALCON 
