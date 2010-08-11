#include "SolidMechTempCoupleFalcon.h"

template<>
InputParameters validParams<SolidMechTempCoupleFalcon>()
{
  InputParameters params = validParams<SolidMechFalcon>();
  return params;
}

SolidMechTempCoupleFalcon::SolidMechTempCoupleFalcon(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :SolidMechFalcon(name, moose_system, parameters),
   _temp_var(coupled("temperature")),
   _thermal_strain(getMaterialProperty<Real>("thermal_strain")),
   _alpha(getMaterialProperty<Real>("alpha"))
{}

void
SolidMechTempCoupleFalcon::recomputeCouplingConstants()
{
  recomputeConstants();
  
  _c4 = _E/(1.-_nu);
  
  if( 3 == _dim )
    _c4 = _E/(1.-2.*_nu);    
}
