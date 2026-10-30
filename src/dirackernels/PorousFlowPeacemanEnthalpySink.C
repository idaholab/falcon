#include "PorousFlowPeacemanEnthalpySink.h"
#include "SinglePhaseFluidProperties.h"
#include "Function.h"

registerMooseObject("FalconApp", PorousFlowPeacemanEnthalpySink);

InputParameters
PorousFlowPeacemanEnthalpySink::validParams()
{
  InputParameters params = PorousFlowPeacemanBorehole::validParams();
  params.addRequiredParam<UserObjectName>(
      "fp",
      "SinglePhaseFluidProperties UserObject used to evaluate the injected fluid's specific "
      "enthalpy from the local pressure and 'injection_temperature'. (Deliberately not "
      "restricted from the name 'fp' - be aware of the same [GlobalParams] name-collision "
      "reason documented on PorousFlowPeacemanBorehole's 'unit_weight_fp' parameter if this app "
      "ever sets a global 'fp'.)");
  params.addRequiredCoupledVar(
      "pressure", "The porepressure variable, at which the injected fluid's enthalpy is evaluated.");
  params.addRequiredParam<FunctionName>(
      "injection_temperature",
      "Temperature (K) of the fluid entering the well at this point. Unlike "
      "PorousFlowPeacemanBorehole's own use_enthalpy=true, which carries the LOCAL SOLVED "
      "field's enthalpy (correct for production), this is a prescribed value - the fluid being "
      "injected carries its own enthalpy, not the local reservoir's.");
  params.addClassDescription(
      "Advective heat source/sink for an injection well: the mass flux computed by "
      "PorousFlowPeacemanBorehole's well-index formula, multiplied by the enthalpy of fluid at "
      "the local pressure and a prescribed injection temperature - the injection-side "
      "counterpart to PorousFlowPeacemanBorehole's own use_enthalpy=true, which is correct for "
      "production but carries the wrong (local, rather than injected) enthalpy for injection.");
  return params;
}

PorousFlowPeacemanEnthalpySink::PorousFlowPeacemanEnthalpySink(const InputParameters & parameters)
  : PorousFlowPeacemanBorehole(parameters),
    _pressure(coupledValue("pressure")),
    _t_in(getFunction("injection_temperature")),
    _fp(getUserObject<SinglePhaseFluidProperties>("fp")),
    _p_var_num(coupled("pressure"))
{
  if (getParam<bool>("use_enthalpy"))
    paramError("use_enthalpy",
               "must be false for PorousFlowPeacemanEnthalpySink: this kernel already multiplies "
               "the Peaceman mass flux by the injected fluid's own enthalpy, so leaving "
               "use_enthalpy=true would multiply by the local solved field's enthalpy on top of "
               "that.");
  if (getParam<MooseEnum>("function_of") != "pressure")
    paramError("function_of",
               "must be 'pressure' for PorousFlowPeacemanEnthalpySink: its mass flux is driven "
               "by the difference between the reservoir porepressure and the wellbore pressure.");
}

Real
PorousFlowPeacemanEnthalpySink::computeQpBaseOutflow(unsigned current_dirac_ptid) const
{
  const Real h = _fp.h_from_p_T(_pressure[_qp], _t_in.value(_t, _q_point[_qp]));
  return PorousFlowPeacemanBorehole::computeQpBaseOutflow(current_dirac_ptid) * h;
}

void
PorousFlowPeacemanEnthalpySink::computeQpBaseOutflowJacobian(unsigned jvar,
                                                             unsigned current_dirac_ptid,
                                                             Real & outflow,
                                                             Real & outflowp) const
{
  // outflow/outflowp here are the *unscaled* Peaceman outflow and its derivative wrt jvar;
  // computeQpBaseOutflow() scales outflow by h, so the same chain rule must be applied here.
  PorousFlowPeacemanBorehole::computeQpBaseOutflowJacobian(jvar, current_dirac_ptid, outflow, outflowp);

  Real h, dh_dp, dh_dT;
  _fp.h_from_p_T(_pressure[_qp], _t_in.value(_t, _q_point[_qp]), h, dh_dp, dh_dT);
  // dh_dT is deliberately unused: injection_temperature is a prescribed Function, not a solution
  // variable, so h has no dependence on whatever is being solved for - which is precisely the
  // difference between this kernel and use_enthalpy=true.

  if (jvar == _p_var_num)
    outflowp = h * outflowp + dh_dp * _phi[_j][_qp] * outflow;
  else
    outflowp *= h;

  outflow *= h;
}
