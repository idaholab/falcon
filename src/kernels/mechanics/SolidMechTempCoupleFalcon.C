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

#include "SolidMechTempCoupleFalcon.h"

template<>
InputParameters validParams<SolidMechTempCoupleFalcon>()
{
  InputParameters params = validParams<SolidMechFalcon>();
  return params;
}

SolidMechTempCoupleFalcon::SolidMechTempCoupleFalcon(const std::string & name, InputParameters parameters)
  :SolidMechFalcon(name, parameters),
   _mesh_dimension(_mesh.dimension()),
   _temp_var(coupled("temperature")),
   _thermal_strain(getMaterialProperty<Real>("thermal_strain")),
   _alpha(getMaterialProperty<Real>("alpha"))
{}

void
SolidMechTempCoupleFalcon::recomputeCouplingConstants()
{
  recomputeConstants();

  _c4 = _E/(1.-_nu);

  if( 3 == _mesh_dimension )
    _c4 = _E/(1.-2.*_nu);
}
