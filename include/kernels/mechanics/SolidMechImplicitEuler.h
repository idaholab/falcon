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

#ifndef SOLIDMECHIMPLICITEULER_H
#define SOLIDMECHIMPLICITEULER_H

#include "SecondDerivativeImplicitEuler.h"
#include "Material.h"


//Forward Declarations
class SolidMechImplicitEuler;

template<>
InputParameters validParams<SolidMechImplicitEuler>();

class SolidMechImplicitEuler : public SecondDerivativeImplicitEuler
{
public:

  SolidMechImplicitEuler(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpJacobian();
  
private:
  MaterialProperty<Real> & _density;
};
#endif //SOLIDMECHIMPLICITEULER_H
