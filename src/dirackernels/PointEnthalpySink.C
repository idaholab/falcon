//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PointEnthalpySink.h"
#include "SinglePhaseFluidProperties.h"

registerMooseObject("FalconApp", PointEnthalpySink);

InputParameters
PointEnthalpySink::validParams()
{
  InputParameters params = DiracKernel::validParams();
  params.addRequiredParam<PostprocessorName>(
      "mass_flux",
      "The postprocessor name holding the mass flux of injected fluid at this point in kg/s "
      "(please ensure this is positive so that this object acts like a source)");
  params.addRequiredParam<UserObjectName>(
      "fp",
      "The name of the user object used to calculate the fluid properties of the injected fluid");
  params.addRequiredCoupledVar(
      "pressure", "Pressure used to calculate the injected fluid enthalpy (measured in Pa)");
/*  params.addRequiredParam<PostprocessorName>(
      "T_in", "The postprocessor name holding the temperature of injected fluid (measured in K)");*/
  params.addRequiredParam<Point>("point", "The x,y,z coordinates of the point source");
  params.addClassDescription("Point source that adds heat energy corresponding to injection of a "
                             "fluid with specified mass flux rate (specified by a postprocessor) "
                             "at given temperature (specified by a postprocessor)");
  return params;
}

PointEnthalpySink::PointEnthalpySink(
    const InputParameters & parameters)
  : DiracKernel(parameters),
    _mass_flux(getPostprocessorValue("mass_flux")),
    _pressure(coupledValue("pressure")),
    //_T_in(getPostprocessorValue("T_in")),
    _temperature(&getMaterialProperty<Real>("PorousFlow_temperature_qp")),
    _fp(getUserObject<SinglePhaseFluidProperties>("fp")),
    _p(getParam<Point>("point")),
    _p_var_num(coupled("pressure"))
    // _has_enthalpy(hasMaterialProperty<std::vector<Real>>("PorousFlow_fluid_phase_enthalpy_nodal") &&
    //               hasMaterialProperty<std::vector<std::vector<Real>>>(
    //                   "dPorousFlow_fluid_phase_enthalpy_nodal_dvar")),
    // _use_enthalpy(getParam<bool>("use_enthalpy")),
    // _enthalpy(_has_enthalpy ? &getMaterialPropertyByName<std::vector<Real>>(
    //                               "PorousFlow_fluid_phase_enthalpy_nodal")
    //                         : nullptr),
    // _denthalpy_dvar(_has_enthalpy ? &getMaterialPropertyByName<std::vector<std::vector<Real>>>(
    //                                     "dPorousFlow_fluid_phase_enthalpy_nodal_dvar")
    //                               : nullptr),
    // _multiplying_var(coupledValue("multiplying_var"))
{
  // To correctly compute the Jacobian terms,
  // tell MOOSE that this DiracKernel depends on all the PorousFlow Variables
//   const std::vector<MooseVariableFEBase *> & coupled_vars = _dictator.getCoupledMooseVars();
//   for (unsigned int i = 0; i < coupled_vars.size(); i++)
//     addMooseVariableDependency(coupled_vars[i]);
}

void
PointEnthalpySink::addPoints()
{
  addPoint(_p, 0);
}

Real
PointEnthalpySink::computeQpResidual()
{
  // Negative sign to make a positive mass_flux in the input file a source
  Real h = _fp.h_from_p_T(_pressure[_qp], (*_temperature)[_qp]);
  return _test[_i][_qp] * _mass_flux * h;

// // Get the ID we initially assigned to this point
//   const unsigned current_dirac_ptid = currentPointCachedID();
//   Real outflow = computeQpBaseOutflow(current_dirac_ptid);
//   if (outflow == 0.0)
//     return 0.0;

//   outflow *= _multiplying_var[_qp];

//   if (_use_enthalpy)
//     outflow *= (*_enthalpy)[_i][_ph];

//   _total_outflow_mass.add(
//       outflow * _dt); // this is not thread safe, but DiracKernel's aren't currently threaded

//   return outflow;

}

Real
PointEnthalpySink::computeQpJacobian()
{
  return 0.;
}

Real
PointEnthalpySink::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _p_var_num)
  {
    Real h, dh_dp, dh_dT;
    _fp.h_from_p_T(_pressure[_qp], (*_temperature)[_qp], h, dh_dp, dh_dT);
    return _test[_i][_qp] * _phi[_j][_qp] * _mass_flux * dh_dp;
  }
  else
    return 0.;
}
