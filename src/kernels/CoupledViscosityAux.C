#include "CoupledViscosityAux.h"

template<>
InputParameters validParams<CoupledViscosityAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.set<Real>("a")=0.0;
  params.set<Real>("b")=0.0;
  params.set<Real>("c")=0.0;
  params.set<Real>("_mu_w")=0.0;
  
  return params;
}

CoupledViscosityAux::CoupledViscosityAux(std::string name, MooseSystem & moose_system, InputParameters parameters)
  :AuxKernel(name, moose_system, parameters),
   _temperature(coupled("temperature")),
   _temperature_val(coupledValAux("temperature")),
   _a(_parameters.get<Real>("a")),
   _b(_parameters.get<Real>("b")),
   _c(_parameters.get<Real>("c")),
   _mu_w(_parameters.get<Real>("_mu_w"))
{}


Real
CoupledViscosityAux::computeValue()
{

  
  if (_temperature_val <= 40.)
  {
      _a = 1.787E-3;
      _b = (-0.03288+(1.962E-4*_temperature_val))*_temperature_val;
      _mu_w = _a * exp(_b);
  }
    
  else if (_temperature_val <= 100.)
  {
      _a = 1e-3;
      _b = (1+(0.015512*(_temperature_val-20)));
      _c = -1.572;
      _mu_w = _a * pow(_b,_c);
  }
    
  else // (_temperature_val <= 300.)
  {
      _a = 0.2414;
      _b = 247 / (_temperature_val+133.15);
      _c = (_a * pow(10,_b));
      _mu_w = _c * 1E-4;
  }
    
  /*

  _a = 1.787E-3;
  _b = (-0.03288+(1.962E-4*_temperature_val))*_temperature_val;
  return _a * exp(_b);
  */
  return (_mu_w);
}
