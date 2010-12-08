#include "SinHeat.h"
 
template<>
InputParameters validParams<SinHeat>()
{
  InputParameters params = validParams<Kernel>();
  params.set<Real>("qdotzero")=0.0;
  params.set<Real>("qmax")=0.0;
  params.set<Real>("duration")=0.0;
  return params;
}

SinHeat::SinHeat(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _qdotzero(getParam<Real>("qdotzero")),
   _qmax(getParam<Real>("qmax")),
   _duration(getParam<Real>("duration"))
{}

Real
SinHeat::computeQpResidual()
{
  
  Real value;

  if(_t < _duration)
    value = _qdotzero + (_qmax - _qdotzero) * std::sin((0.5/_duration) * libMesh::pi * _t);
  else
    value = _qmax;
    
  return -_phi[_i][_qp]*value;
}
