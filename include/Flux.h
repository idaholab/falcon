#ifndef FLUX_H
#define FLUX_H

#include "BoundaryCondition.h"

//Forward Declarations
class Flux;

template<>
InputParameters validParams<Flux>();

class Flux : public BoundaryCondition
{
public:

  Flux(std::string name, InputParameters parameters, std::string var_name, unsigned int boundary_id, std::vector<std::string> coupled_to, std::vector<std::string> coupled_as);
  
  virtual ~Flux(){}

protected:
  virtual Real computeQpResidual(){return 0;}
  virtual Real computeQpIntegral();
  
private:
  Real _coefficient;
};

#endif //FLUXBC_H

