#ifndef SINHEAT_H
#define SINHEAT_H

#include "Kernel.h"
#include "libmesh_common.h"


//Forward Declarations
class SinHeat;

template<>
InputParameters validParams<SinHeat>();

class SinHeat : public Kernel
{
public:

  SinHeat(std::string name, MooseSystem & moose_system, InputParameters parameters);
  
protected:
  virtual Real computeQpResidual();
 
private:
  Real _qdotzero;
  Real _qmax;
  Real _duration;
};
 
#endif
