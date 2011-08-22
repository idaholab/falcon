#include "TimeDerivative.h"

#ifndef MASSFLUXTIMEDERIVATIVE
#define MASSFLUXTIMEDERIVATIVE

//Forward Declarations
class MassFluxTimeDerivative;

template<>
InputParameters validParams<MassFluxTimeDerivative>();

class MassFluxTimeDerivative : public TimeDerivative
{
public:

  MassFluxTimeDerivative(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  
  VariableValue  & _density;
  VariableValue & _density_old;
  VariableValue  & _ddensitydp_H; //derivative of water density vs. pressure
  VariableValue & _enthalpy_old;
  unsigned int _h_var;
  VariableValue & _ddensitydH_P;
  
    
  MaterialProperty<Real> & _porosity;  
//  VariableValue  & _porosity;
//  VariableValue  & _porosity_old;

};
#endif //MASSFLUXTIMEDERIVATIVE
