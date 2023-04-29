/*
AuxKernel of Passing Variable
*/

#pragma once

#include "AuxKernel.h"

class CompVar : public AuxKernel
{
    public:

    static InputParameters validParams();
    CompVar(const InputParameters & parameters);

    protected:

    virtual Real computeValue() override;

    const VariableValue & _coupled_val;

};



