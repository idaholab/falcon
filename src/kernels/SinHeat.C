#include "SinHeat.h"
 
template<>
Parameters valid_params<SinHeat>()
{
  Parameters params;
  params.set<Real>("qdotzero")=0.0;
  params.set<Real>("qmax")=0.0;
  params.set<Real>("duration")=0.0;
  return params;
}

SinHeat::SinHeat(std::string name,
            Parameters parameters,
            std::string var_name,
            std::vector<std::string> coupled_to,
            std::vector<std::string> coupled_as)
    :Kernel(name,parameters,var_name,true,coupled_to,coupled_as),
    _qdotzero(_parameters.get<Real>("qdotzero")),
    _qmax(_parameters.get<Real>("qmax")),
    _duration(_parameters.get<Real>("duration"))
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
