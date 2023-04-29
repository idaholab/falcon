/*
AuxKernel of Passing Variable Time Derivative
*/

#pragma once

#include "AuxKernel.h"

class CompVarRate : public AuxKernel
{
    public:

    static InputParameters validParams();
    CompVarRate(const InputParameters & parameters);

    protected:

    virtual Real computeValue() override;

    const VariableValue & _coupled_val;

};



