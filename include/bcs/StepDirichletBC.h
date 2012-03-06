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

#ifndef STEPDIRICHLETBC_H
#define STEPDIRICHLETBC_H

#include "libmesh_common.h"
#include "NodalBC.h"


//Forward Declarations
class StepDirichletBC;

template<>
InputParameters validParams<StepDirichletBC>();

class StepDirichletBC : public NodalBC
{
public:

  StepDirichletBC(const std::string & name, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();

private:
  Real _size;
  std::vector<Real> _time;
  std::vector<Real> _value;  
};
 
#endif
