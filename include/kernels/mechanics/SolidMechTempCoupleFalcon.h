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

#ifndef SOLIDMECHTEMPCOUPLEFALCON_H
#define SOLIDMECHTEMPCOUPLEFALCON_H

#include "SolidMechFalcon.h"

//libMesh includes
#include "libmesh/tensor_value.h"


//Forward Declarations
class SolidMechTempCoupleFalcon;

template<>
InputParameters validParams<SolidMechTempCoupleFalcon>();

class SolidMechTempCoupleFalcon : public SolidMechFalcon
{
public:

  SolidMechTempCoupleFalcon(const std::string & name, InputParameters parameters);
  
  void recomputeCouplingConstants();
  
protected:
  unsigned int _temp_var;
  
  MaterialProperty<Real> & _thermal_strain;
  MaterialProperty<Real> & _alpha;

  Real _c4;
};
 

#endif //SOLIDMECHTEMPCOUPLEFALCON_H
