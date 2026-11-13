#pragma once

#include "PorousFlowPeacemanBorehole.h"

class SinglePhaseFluidProperties;
class Function;

/**
 * Advective heat source/sink for an injection well: the mass flux computed by
 * PorousFlowPeacemanBorehole's own well-index formula, multiplied by the enthalpy of fluid at
 * the local pressure and a prescribed injection temperature.
 *
 * This is the injection-side counterpart to PorousFlowPeacemanBorehole's own use_enthalpy=true
 * mode: that mode multiplies by the LOCAL SOLVED field's own enthalpy, which is correct for
 * production (the fluid leaving the formation genuinely carries the formation's own enthalpy)
 * but wrong for injection (the fluid entering carries the INJECTED fluid's enthalpy, not the
 * local reservoir's). FALCON's EnthalpySink already implements exactly this "multiply by a fixed
 * inlet temperature's enthalpy" idea, but on top of PorousFlowPolyLineSink, whose own mass flux
 * comes from a prescribed p_or_t_vals/fluxes table rather than Peaceman's well-index formula.
 * This class applies the same pattern on top of PorousFlowPeacemanBorehole instead, so the mass
 * side can be rate-controlled (see PorousFlowRateControlledBoreholePressure) and its permeability
 * -based partitioning across points carries over unchanged.
 */
class PorousFlowPeacemanEnthalpySink : public PorousFlowPeacemanBorehole
{
public:
  static InputParameters validParams();

  PorousFlowPeacemanEnthalpySink(const InputParameters & parameters);

  virtual Real computeQpBaseOutflow(unsigned current_dirac_ptid) const override;
  virtual void computeQpBaseOutflowJacobian(unsigned jvar,
                                            unsigned current_dirac_ptid,
                                            Real & outflow,
                                            Real & outflowp) const override;

protected:
  /// Porepressure, at which the injected fluid's enthalpy is evaluated
  const VariableValue & _pressure;

  /// Temperature (K) of the fluid entering the well at this point. Deliberately a Function, not
  /// PorousFlowPeacemanBorehole's own use_enthalpy=true local-field behavior: the enthalpy
  /// carried is the INJECTED fluid's, prescribed independently of whatever the local reservoir
  /// temperature is.
  const Function & _t_in;

  /// Fluid properties used to evaluate the injected fluid's specific enthalpy
  const SinglePhaseFluidProperties & _fp;

  /// Pressure variable number (for the enthalpy dh/dp Jacobian term)
  unsigned int _p_var_num;
};
