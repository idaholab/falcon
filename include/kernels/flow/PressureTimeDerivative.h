#include "TimeDerivative.h"

#ifndef PRESSURETIMEDERIVATIVE
#define PRESSURETIMEDERIVATIVE

//Forward Declarations
class PressureTimeDerivative;

template<>
InputParameters validParams<PressureTimeDerivative>();

class PressureTimeDerivative : public TimeDerivative
{
public:
    
    PressureTimeDerivative(const std::string & name, InputParameters parameters);
    
protected:
    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();
    
     MaterialProperty<Real> & _porosity;  
     MaterialProperty<Real> & _compressibility;
    //  VariableValue  & _porosity;
    //  VariableValue  & _porosity_old;
    
};
#endif //MASSFLUXTIMEDERIVATIVE
