#include "LinearDisEnthalpyIC.h"

template<>
InputParameters validParams<LinearDisEnthalpyIC>()
{

  InputParameters params= validParams<InitialCondition>();
  params.addRequiredParam<Real>("basevalue","the value of the interception of enthalpy with x equaling zero");

  params.addRequiredParam<Real>("slope","the changing rate of enthalpy with space");

  return params;
}

LinearDisEnthalpyIC::LinearDisEnthalpyIC(const std::string & name, InputParameters parameters)
    :InitialCondition(name,parameters),
     _basevalue(getParam<Real>("basevalue")),
     _slope(getParam<Real> ("slope"))
{
}


Real
LinearDisEnthalpyIC::value(const Point & p)
{
  return _basevalue+_slope*p(0);
}
