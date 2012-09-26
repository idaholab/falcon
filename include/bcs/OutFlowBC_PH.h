/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef OUTFLOWBC_PH_H
#define OUTFLOWBC_PH_H

#include "IntegratedBC.h"
#include "Material.h"
//libMesh includes
#include "vector_value.h"


//Forward Declarations
class OutFlowBC_PH;

template<>
InputParameters validParams<OutFlowBC_PH>();

/**
 * Implements a simple constant VectorNeumann BC where grad(u)=value on the boundary.
 * Uses the term produced from integrating the diffusion operator by parts.
 */
class OutFlowBC_PH : public IntegratedBC
{
public:
    
    /**
     * Factory constructor, takes parameters so that all derived classes can be built using the same
     * constructor.
     */
    OutFlowBC_PH(const std::string & name, InputParameters parameters);
    
    virtual ~OutFlowBC_PH(){}
    
protected:
    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();
    virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  
    
private:
    /**
     * Vector to dot with the normal.
     */
    
    VariableGradient & _grad_p;
    unsigned int _p_var;
    VariableGradient & _grad_T;

    MaterialProperty<Real> & _dTdH;
    MaterialProperty<Real> & _dTdP;
    MaterialProperty<Real> & _enthalpy_water;
    MaterialProperty<Real> & _enthalpy_steam;
    MaterialProperty<Real> &_denthalpy_waterdH_P;
    MaterialProperty<Real> &_denthalpy_steamdH_P;
    MaterialProperty<Real> &_denthalpy_waterdP_H;
    MaterialProperty<Real> &_denthalpy_steamdP_H;
    
    MaterialProperty<Real> & _Dtau_waterDP;
    MaterialProperty<Real> & _Dtau_waterDH;
    MaterialProperty<Real> & _Dtau_steamDP;
    MaterialProperty<Real> & _Dtau_steamDH;
    MaterialProperty<Real> & _tau_water;
    MaterialProperty<Real> & _tau_steam;
    MaterialProperty<RealGradient> & _darcy_mass_flux_water;
    MaterialProperty<RealGradient> & _darcy_mass_flux_steam;
    MaterialProperty<Real> & _thermal_conductivity;
    
    //  std::vector<RealGradient> & _grad_p;
    
};

#endif //NEUMANNBC_H
