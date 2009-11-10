#ifndef SINHEAT_H
#define SINHEAT_H

#include "Kernel.h"
#include "libmesh_common.h"


//Forward Declarations
class SinHeat;

template<>
InputParameters valid_params<SinHeat>();

class SinHeat : public Kernel
{
public:

  SinHeat(std::string name,
            InputParameters parameters,
            std::string var_name,
            std::vector<std::string> coupled_to=std::vector<std::string>(0),
          std::vector<std::string> coupled_as=std::vector<std::string>(0));
  
protected:
  virtual Real computeQpResidual();
 
private:
  Real _qdotzero;
  Real _qmax;
  Real _duration;
};
 
#endif
