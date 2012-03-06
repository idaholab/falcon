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

#include "Material.h"
#include "CoupledPorosityMaterialAux.h"

template<>
InputParameters validParams<CoupledPorosityMaterialAux>()
{
  InputParameters params = validParams<AuxKernel>();
//  params.addRequiredCoupledVar("temperature", "Use temperature to calculate variable density and viscosity");
//  params.addParam<bool>("temp_dependent_viscosity", true, "Flag to call density and viscosity routine");


  // params.addParam<Real>("porosity", 0.3,"Initial reservoir porosity");
  return params;
}

CoupledPorosityMaterialAux::CoupledPorosityMaterialAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
   //  _temperature(coupledValue("temperature")),
   // _input_porosity(getParam<Real>("porosity"))
   
     _input_porosity(getMaterialProperty<Real>("material_porosity"))
   
//   _has_variable_viscosity(getParam<bool>("temp_dependent_viscosity"))

{}


Real
CoupledPorosityMaterialAux::computeValue()
{
return _input_porosity[_qp];
}


