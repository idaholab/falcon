#ifndef COUPLEDTEMPERATUREAUX_H
#define COUPLEDTEMPERATUREAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledTemperatureAux;

template<>
InputParameters validParams<CoupledTemperatureAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledTemperatureAux : public AuxKernel
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    CoupledTemperatureAux(const std::string & name, InputParameters parameters);
    
    virtual ~CoupledTemperatureAux() {}
    
protected:
    virtual Real computeValue();
    
    VariableValue & _pressure;
    VariableValue & _enthalpy;
    VariableValue & _sw;
    VariableValue & _den_mix;
    VariableValue & _den_water;
    VariableValue & _den_steam;
    VariableValue & _enthalpy_water;
    VariableValue & _enthalpy_steam;
    VariableValue & _ddendh_P;
    VariableValue & _ddendp_H;
    VariableValue & _denthalpy_waterdH_P;
    VariableValue & _denthalpy_steamdH_P;
    VariableValue & _dTdH_P;
    VariableValue & _dswdH;
    Real _input_temperature;
                     
};

#endif //COUPLEDDDENSITYDTAUX_H
