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

#include "StressStrainDamageComputeAux.h"

template<>
InputParameters validParams<StressStrainDamageComputeAux>()
{
    InputParameters params = validParams<AuxKernel>();
    params.addParam<std::string>("quantity","stress", "output: stress, strain, pstress, pstrain, damage or permeability.");
    params.addParam<int>("component",0,"component of the stress/strain vector, 0-xx;1-yy;2-zz;3-xy;4-xz;5-yz");
    return params;
}

StressStrainDamageComputeAux::StressStrainDamageComputeAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
    _mesh_dimension(_mesh.dimension()),

    _quantity_string( getParam<std::string>("quantity") ),
    _component(getParam<int>("component")),

    _stress_normal_vector(getMaterialProperty<RealVectorValue>("stress_normal_vector")),
    _stress_shear_vector(getMaterialProperty<RealVectorValue>("stress_shear_vector")),
    _strain_normal_vector(getMaterialProperty<RealVectorValue>("strain_normal_vector")),
    _strain_shear_vector(getMaterialProperty<RealVectorValue>("strain_shear_vector"))

{}

Real
StressStrainDamageComputeAux::computeValue()
{
    std::transform(_quantity_string.begin(), _quantity_string.end(),
                   _quantity_string.begin(), ::tolower);
    
    if ( _quantity_string == "stress")
    {
        //stress_normal (tau_xx and tau_yy)
        _stress(0) = _stress_normal_vector[_qp](0);
        _stress(1) = _stress_normal_vector[_qp](1);
        //if 3D problem, stress_normal in z-direction (tau_zz)
        if (_mesh_dimension == 3)
            _stress(2) = _stress_normal_vector[_qp](2); //tau_zz
        
        //stress_shear (tau_xy)
        _stress(3) = _stress_shear_vector[_qp](0);
        //if 3D problem, stress_shear in z_directions (tau_xz and tau_yz)
        if (_mesh_dimension == 3)
        {
            _stress(4) =  _stress_shear_vector[_qp](1);
            _stress(5) =  _stress_shear_vector[_qp](2);
        }
        return _stress(_component);

    }
    else if ( _quantity_string == "strain" )
    {
        //strain_normal (s_xx and s_yy)
        _strain(0) = _strain_normal_vector[_qp](0);
        _strain(1) = _strain_normal_vector[_qp](1);
        //if 3D problem, strain_normal in z_direction (s_zz)
        if (_mesh_dimension == 3)
            _strain(2) = _strain_normal_vector[_qp](2);
        
        //strain_shear (s_xy)
        _strain(3) = _strain_shear_vector[_qp](0);
        //if 3D problem, strain_shear in z_directions (s_xz and s_yz)
        if (_mesh_dimension == 3)
        {
            _strain(4) =  _strain_shear_vector[_qp](1);
            _strain(5) =  _strain_shear_vector[_qp](2);
        }
        return _strain(_component);
    }
    return 0.0;
}
