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
#include "libmesh/vector_value.h"


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

    const MaterialProperty<Real> & _dTdH;
    const MaterialProperty<Real> & _dTdP;
    const MaterialProperty<Real> & _enthalpy_water;
    const MaterialProperty<Real> & _enthalpy_steam;
    const MaterialProperty<Real> &_denthalpy_waterdH_P;
    const MaterialProperty<Real> &_denthalpy_steamdH_P;
    const MaterialProperty<Real> &_denthalpy_waterdP_H;
    const MaterialProperty<Real> &_denthalpy_steamdP_H;

    const MaterialProperty<Real> & _Dtau_waterDP;
    const MaterialProperty<Real> & _Dtau_waterDH;
    const MaterialProperty<Real> & _Dtau_steamDP;
    const MaterialProperty<Real> & _Dtau_steamDH;
    const MaterialProperty<Real> & _tau_water;
    const MaterialProperty<Real> & _tau_steam;
    const MaterialProperty<RealGradient> & _darcy_mass_flux_water;
    const MaterialProperty<RealGradient> & _darcy_mass_flux_steam;
    const MaterialProperty<Real> & _thermal_conductivity;

    //  std::vector<RealGradient> & _grad_p;

};

#endif //NEUMANNBC_H
