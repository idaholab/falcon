#include "TimeDerivative.h"

#ifndef MASSFLUXTIMEDERIVATIVE_PT
#define MASSFLUXTIMEDERIVATIVE_PT

//Forward Declarations
class MassFluxTimeDerivative_PT;

template<>
InputParameters validParams<MassFluxTimeDerivative_PT>();

class MassFluxTimeDerivative_PT : public TimeDerivative
{
public:

  MassFluxTimeDerivative_PT(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  VariableValue & _density_water;
  VariableValue & _density_water_old;
  VariableValue & _dwdp; //derivative of water density vs. pressure  
    
  MaterialProperty<Real> & _porosity;  
//  VariableValue  & _porosity;
//  VariableValue  & _porosity_old;

};
#endif //MASSFLUXTIMEDERIVATIVE_PT
