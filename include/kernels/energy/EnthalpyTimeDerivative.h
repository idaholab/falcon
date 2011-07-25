#include "TimeDerivative.h"

#ifndef ENTHALPYTIMEDERIVATIVE
#define ENTHALPYTIMEDERIVATIVE

//Forward Declarations
class EnthalpyTimeDerivative;

template<>
InputParameters validParams<EnthalpyTimeDerivative>();

class EnthalpyTimeDerivative : public TimeDerivative
{
public:
    
    EnthalpyTimeDerivative(const std::string & name, InputParameters parameters);
    
protected:
    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();
    
    
    VariableValue  & _density;
    VariableValue  & _density_old;
    VariableValue  & _temperature;
    VariableValue  & _temperature_old;
    VariableValue  & _dTdH_P; //derivative of water density vs. temperature
    VariableValue  & _ddensitydH_P;
    VariableValue  & _pressure_old;
    //  VariableValue  & _porosity_old;
    MaterialProperty<Real> & _porosity;
    MaterialProperty<Real> & _specific_heat_rock;
    MaterialProperty<Real> & _density_rock;
    
};
#endif //ENTHALPYTIMEDERIVATIVE
