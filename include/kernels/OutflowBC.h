#ifndef OUTFLOWBC
#define OUTFLOWBC

#include "BoundaryCondition.h"

//libMesh includes
#include "vector_value.h"


//Forward Declarations
class OutFlowBC;

template<>
InputParameters validParams<OutFlowBC>();


class OutFlowBC : public BoundaryCondition
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  OutFlowBC(std::string name, InputParameters parameters, std::string var_name, unsigned int boundary_id, std::vector<std::string> coupled_to, std::vector<std::string> coupled_as);
  
 virtual ~OutFlowBC(){}

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  
  Real _water_specific_heat;
  Real _rho_w;
  
  RealVectorValue _darcy_velocity;
  
private:

};

#endif //OUTFLOWBC
