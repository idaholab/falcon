#include "PorousFlowCasedBoreholeHeatExchange.h"
#include "Function.h"
#include "SinglePhaseFluidProperties.h"
#include "PorousFlowSumQuantity.h"
#include "PorousFlowPointFluxQuantity.h"
#include "SystemBase.h"
#include "libmesh/system.h"

registerMooseObject("FalconApp", PorousFlowCasedBoreholeHeatExchange);

InputParameters
PorousFlowCasedBoreholeHeatExchange::validParams()
{
  InputParameters params = PorousFlowLineGeometry::validParams();
  // The march in computeWellboreTemperatures() samples every well point's temperature directly
  // (not just the ones that ended up with a Dirac source on this processor), so a missing point
  // must not be silently dropped - see PorousFlowPeacemanBorehole::validParams() for the
  // identical reasoning.
  params.set<MooseEnum>("point_not_found_behavior") = "ERROR";
  params.addRequiredParam<FunctionName>(
      "character",
      "Nonzero over the cased section of the well, where this kernel is active (the exchanged "
      "heat rate is multiplied by |character|); zero over the open/perforated section, where "
      "PorousFlowPeacemanBorehole already exchanges heat advectively.");
  params.addRequiredRangeCheckedParam<Real>(
      "heat_transfer_coefficient",
      "heat_transfer_coefficient > 0",
      "Casing-side heat transfer coefficient (W/m^2/K), referenced to the casing outer radius: "
      "the combined resistance of the in-well convective film, the steel casing, and the cement "
      "sheath. This is NOT an overall wellbore-to-far-field U - the formation's own conductive "
      "resistance is already resolved by the mesh, so including it here would double-count it.");
  params.addRequiredParam<VectorPostprocessorName>(
      "mass_point_flux_vpp",
      "The name of a PorousFlowPlotPointFluxQuantity VectorPostprocessor reporting the mass flux "
      "(its 'flux' vector) at each point of the open/perforated section - produced, in "
      "production mode; injected, in injection mode. Its point set is independent of this "
      "kernel's own: in production mode the mixing temperature is evaluated at that VPP's own "
      "reported 'x'/'y'/'z' coordinates, so the cased section may place its points however it "
      "likes (see PolylineDiracPoints). By convention the cased path's deepest point should "
      "coincide with the shallowest point of the open interval, so that the distance along the "
      "well in the wellbore-temperature march is measured from the true open/cased boundary - "
      "enforced at initialSetup() by requiring 'character' to be zero at this kernel's own last "
      "point. Used to get the total mass rate in both modes, and (production only) to compute "
      "the mass-flux-weighted mixing temperature of the fluid entering the cased section - the "
      "fluid produced deeper in the open interval is hotter, so a mixing average is used rather "
      "than the local formation temperature at the open/cased boundary alone.");
  MooseEnum flow_direction("production injection", "production");
  params.addParam<MooseEnum>(
      "flow_direction",
      flow_direction,
      "Which way fluid flows through the cased section. 'production' (the default): fluid "
      "enters at the open/cased boundary carrying the mass-flux-weighted mixing temperature of "
      "everything produced across the open interval, and flows UP to the wellhead. 'injection': "
      "fluid enters at the WELLHEAD at 'injection_temperature' and flows DOWN to the open/cased "
      "boundary, so the march runs in the opposite index direction and the entry condition is "
      "prescribed rather than derived.");
  params.addParam<FunctionName>(
      "injection_temperature",
      "Temperature (K) of the fluid entering the cased section at the wellhead. Required if and "
      "only if flow_direction = injection. Deliberately NOT fed from the open interval's own "
      "downstream temperature - this is the fluid's temperature before it has exchanged any "
      "heat with the formation on its way down, which is exactly what this kernel then models.");
  params.addRequiredParam<UserObjectName>(
      "wellbore_fp",
      "SinglePhaseFluidProperties UserObject used to evaluate the in-well fluid specific heat "
      "from 'wellbore_reference_pressure' and the current well temperature at each point. "
      "(Deliberately not named 'fp', for the same [GlobalParams] name-collision reason "
      "documented on PorousFlowPeacemanBorehole's 'unit_weight_fp' parameter.)");
  params.addRequiredRangeCheckedParam<Real>(
      "wellbore_reference_pressure",
      "wellbore_reference_pressure > 0",
      "The fixed pressure (Pa) at which the in-well fluid specific heat is evaluated.");
  params.addRequiredParam<UserObjectName>(
      "SumQuantityUO",
      "User Object of type=PorousFlowSumQuantity in which to place the total heat exchanged "
      "with the formation for each time step.");
  params.addRequiredParam<UserObjectName>(
      "PointFluxUO",
      "User Object of type=PorousFlowPointFluxQuantity in which to record the instantaneous "
      "heat rate exchanged with the formation at each individual Dirac point of this kernel, as "
      "computed during the most recent residual evaluation. Use a "
      "PorousFlowPlotPointFluxQuantity VectorPostprocessor to output the recorded values.");
  params.addClassDescription(
      "Conductive heat exchange between a cased wellbore and the surrounding formation, over "
      "the section of a well where casing prevents fluid mass from crossing between the well "
      "and the formation, complementing PorousFlowPeacemanBorehole's advective-only exchange "
      "over the open (perforated) section.");
  return params;
}

PorousFlowCasedBoreholeHeatExchange::PorousFlowCasedBoreholeHeatExchange(
    const InputParameters & parameters)
  : PorousFlowLineGeometry(parameters),
    _character(getFunction("character")),
    _h(getParam<Real>("heat_transfer_coefficient")),
    _flow_direction(getParam<MooseEnum>("flow_direction").getEnum<FlowDirection>()),
    _t_inj(isParamValid("injection_temperature") ? &getFunction("injection_temperature")
                                                 : nullptr),
    _mass_flux(getVectorPostprocessorValue("mass_point_flux_vpp", "flux")),
    _mass_flux_x(getVectorPostprocessorValue("mass_point_flux_vpp", "x")),
    _mass_flux_y(getVectorPostprocessorValue("mass_point_flux_vpp", "y")),
    _mass_flux_z(getVectorPostprocessorValue("mass_point_flux_vpp", "z")),
    _fp(getUserObject<SinglePhaseFluidProperties>("wellbore_fp")),
    _reference_pressure(getParam<Real>("wellbore_reference_pressure")),
    _temperature_system(_var.sys().system()),
    _temperature_var_number(_var.number()),
    _total_outflow(
        const_cast<PorousFlowSumQuantity &>(getUserObject<PorousFlowSumQuantity>("SumQuantityUO"))),
    _point_fluxes(const_cast<PorousFlowPointFluxQuantity &>(
        getUserObject<PorousFlowPointFluxQuantity>("PointFluxUO")))
{
  if (_flow_direction == FlowDirection::injection && !_t_inj)
    paramError("injection_temperature",
               "is required when flow_direction = injection: the cased section's entry "
               "temperature is prescribed at the wellhead, not derived from the open interval's "
               "mixing temperature as it is for production.");
  if (_flow_direction == FlowDirection::production && _t_inj)
    paramError("injection_temperature",
               "is only meaningful when flow_direction = injection. For production the entry "
               "temperature is the mass-flux-weighted mixing temperature derived from "
               "'mass_point_flux_vpp'.");
}

void
PorousFlowCasedBoreholeHeatExchange::initialSetup()
{
  PorousFlowLineGeometry::initialSetup();

  if (_z_coord->size() < 2)
    mooseError("PorousFlowCasedBoreholeHeatExchange: at least two well points are required to "
               "march the wellbore-fluid-temperature profile along a segment.");

  // The wellbore-temperature march needs a point just below the cased section, at the open/cased
  // boundary, to enter the mixing temperature at and to measure cumulative distance from - so the
  // *deepest* point of this kernel's own path must be one where 'character' is zero. Without
  // this check, a path that ended even slightly above that boundary would leave every point
  // active, computeWellboreTemperatures()'s 'i_start + 1 >= num_pts' guard would fire on every
  // call, and this kernel would silently exchange no heat at all - the failure mode this guard
  // exists to catch is exactly the one a mismatch between this kernel's own point placement (see
  // PolylineDiracPoints) and the open interval's point_file would otherwise produce silently.
  const std::size_t num_pts = _z_coord->size();
  int i_start = -1;
  for (int i = static_cast<int>(num_pts) - 1; i >= 0; --i)
  {
    const Point p(_x_coord->at(i), _y_coord->at(i), _z_coord->at(i));
    if (_character.value(_t, p) != 0.0)
    {
      i_start = i;
      break;
    }
  }

  if (i_start < 0)
    paramError("character",
               "'character' is zero at every point of this kernel's path, so it would exchange "
               "no heat anywhere. It must be nonzero over the cased section.");

  if (static_cast<std::size_t>(i_start) + 1 >= num_pts)
  {
    const Point last(
        _x_coord->at(num_pts - 1), _y_coord->at(num_pts - 1), _z_coord->at(num_pts - 1));
    paramError(
        "character",
        "'character' is nonzero at the deepest point of this kernel's path (",
        last,
        "), so there is no point at the open/cased boundary for the wellbore-temperature march "
        "to start from and this kernel would silently exchange no heat. Extend the path so its "
        "last point reaches (or passes) the boundary where 'character' becomes zero - "
        "conventionally the shallowest point of the open interval's own point set. (Checked at "
        "the initial time; a time-varying 'character' is not supported by this check.)");
  }
}

void
PorousFlowCasedBoreholeHeatExchange::residualSetup()
{
  PorousFlowLineGeometry::residualSetup();
  computeWellboreTemperatures();
}

void
PorousFlowCasedBoreholeHeatExchange::jacobianSetup()
{
  PorousFlowLineGeometry::jacobianSetup();
  computeWellboreTemperatures();
}

void
PorousFlowCasedBoreholeHeatExchange::addPoints()
{
  // This gets called just before the DiracKernel is evaluated, so this is a handy place to zero
  // this out - same pattern as PorousFlowLineSink::addPoints().
  _total_outflow.zero();
  _point_fluxes.zero(*_x_coord, *_y_coord, *_z_coord);

  PorousFlowLineGeometry::addPoints();
}

void
PorousFlowCasedBoreholeHeatExchange::computeWellboreTemperatures()
{
  // Sampled from the *old* (previous time step's converged) solution, not the current
  // nonlinear iterate, exactly like mass_point_flux_vpp already is. This makes _t_well a true
  // constant throughout a given time step's whole Newton solve - it no longer responds at all
  // to how the current iterate changes between iterations - so computeQpJacobian's local-only
  // derivative (treating _t_well as fixed) is then *exact* for that solve, not merely the
  // dominant term of an otherwise-undifferentiated dependency. Sampling the current iterate
  // instead was tried first: since _t_well's dependence on temperature elsewhere along the well
  // is comparable in size to the one local term the Jacobian does capture (unlike
  // PorousFlowPeacemanBorehole's much weaker, smoothly-varying bh_pressure-on-temperature
  // dependence), the resulting Jacobian was not just "mildly slower to converge" but
  // structurally missing a same-order-of-magnitude term - which destabilized the coupled Newton
  // solve during this model's production ramp-up, regardless of heat_transfer_coefficient's
  // magnitude, since every version of that dependency shares the same h*perimeter*length
  // prefactor.
  const NumericVector<Number> & old_solution = _var.sys().solutionOld();
  const std::size_t num_pts = _z_coord->size();
  std::vector<Real> temperature_f(num_pts);
  for (const auto i : make_range(num_pts))
  {
    const Point p(_x_coord->at(i), _y_coord->at(i), _z_coord->at(i));
    temperature_f[i] = _temperature_system.point_value(_temperature_var_number, p, &old_solution);
  }

  // Default: outside the march (or if there is nothing to march), the "wellbore fluid
  // temperature" is simply the local formation temperature - harmless, since computeQpResidual
  // returns zero wherever _character is zero anyway.
  _t_well = temperature_f;

  // mass_point_flux_vpp's underlying vectors are cleared to size 0 by its own initialize() and
  // only refilled by its own execute(), on its own schedule - before its first execute() (eg
  // going into the very first real solve), or transiently between initialize() and execute(),
  // they can be empty. Treat that exactly like "no produced flow yet". The coordinate vectors
  // are always filled in lockstep with 'flux' by PorousFlowPlotPointFluxQuantity, so a size
  // mismatch here means the VPP is mid-update, not misconfigured.
  const std::size_t num_mass_pts = _mass_flux.size();
  if (num_mass_pts == 0 || _mass_flux_x.size() != num_mass_pts ||
      _mass_flux_y.size() != num_mass_pts || _mass_flux_z.size() != num_mass_pts)
    return;

  Real mdot = 0.0;
  Real t_entry = 0.0;

  if (_flow_direction == FlowDirection::production)
  {
    // Total produced mass rate, and the mass-flux-weighted mixing temperature of everything
    // produced across the open interval - the fluid entering deeper in that interval is hotter,
    // so this mixing average (not the local formation temperature at the open/cased boundary
    // alone) is the physically correct temperature of the fluid entering the cased section.
    // Sampled at the open interval's OWN reported point coordinates: this kernel's cased section
    // places its points independently (see PolylineDiracPoints), so its own
    // _x_coord/_y_coord/_z_coord no longer index the same physical locations as _mass_flux.
    Real mdot_t = 0.0;
    for (const auto i : make_range(num_mass_pts))
    {
      // Zero-flux points (everywhere the open interval's own 'character' is zero) contribute
      // nothing to either sum, so skipping them is numerically exact - and it avoids a needless
      // System::point_value() call, which insists the point be evaluable and can trip a libMesh
      // assertion in a debug build if it is not. _mass_flux is replicated (its underlying
      // PorousFlowPointFluxQuantity sums across processors in finalize()), so every rank makes
      // the same skip decision and the collective point_value() calls below stay in lockstep.
      if (_mass_flux[i] == 0.0)
        continue;

      const Point p(_mass_flux_x[i], _mass_flux_y[i], _mass_flux_z[i]);
      const Real t = _temperature_system.point_value(_temperature_var_number, p, &old_solution);
      mdot += _mass_flux[i];
      mdot_t += _mass_flux[i] * t;
    }
    if (mdot <= 0.0)
      return; // no produced flow yet (eg the first time step, before mass_point_flux_vpp has run)

    t_entry = mdot_t / mdot;
  }
  else
  {
    // Injection: the entry temperature is PRESCRIBED at the wellhead, so there is no mixing
    // average to form, and none of the per-point formation-temperature sampling the production
    // branch needs either. Only the total injected mass rate is required, and the open interval
    // reports it with the opposite sign (PorousFlow's outflow convention: negative outflow =
    // source, driven by the injector's character < 0).
    for (const auto i : make_range(num_mass_pts))
      mdot += _mass_flux[i];
    mdot = -mdot; // now positive, as this ODE's mdot must be
    if (mdot <= 0.0)
      return; // no injected flow yet (eg the very first timestep)

    // Evaluated at the shallowest point of this kernel's own path - the wellhead - so that a
    // spatially-varying Function reads sensibly. Evaluated at the current time _t, which (like
    // _character and PorousFlowPeacemanBorehole's _p_bot) is deterministic and constant through
    // the whole Newton solve, so _t_well remains a true constant for that solve.
    t_entry = _t_inj->value(
        _t, Point(_x_coord->at(0), _y_coord->at(0), _z_coord->at(0)));
  }

  // Find the deepest point at which this kernel is active (the largest index with nonzero
  // _character): the point_file convention (shared with PorousFlowPeacemanBorehole) is that the
  // *last* point is the well bottom, so the cased section - being shallower than the open
  // section - occupies the lowest-indexed run of points, and i_start is the cased point
  // immediately adjacent to the open interval below it.
  int i_start = -1;
  for (int i = static_cast<int>(num_pts) - 1; i >= 0; --i)
  {
    const Point p(_x_coord->at(i), _y_coord->at(i), _z_coord->at(i));
    if (_character.value(_t, p) != 0.0)
    {
      i_start = i;
      break;
    }
  }
  if (i_start < 0 || static_cast<std::size_t>(i_start) + 1 >= num_pts)
    return; // this kernel is disabled everywhere, or has no open interval below it to mix from

  // Bulk-averaged formation temperature over the cased section (points 0..i_start, plus the
  // open/cased boundary point i_start+1), and the fluid's specific heat evaluated once at the
  // entry temperature - both single, well-mixed values rather than a segment-by-segment
  // recomputation, so T_well at any point depends only on t_entry and this point's own
  // cumulative distance from where the fluid enters, never on another cased point's T_well.
  //
  // temperature_f, t_entry, and t_avg are all sampled from (or, for injection's prescribed
  // t_entry, evaluated at) the *old* (previous time step's converged) solution/time (see above),
  // so none of this is reachable with an unphysical intermediate value from the current Newton
  // iterate. It is still clamped defensively: the
  // converged solution driving next time step's old solution is itself extrapolated by the
  // time integrator/time stepper from a finite-precision solve, and clamping costs nothing on
  // the physically-sane values this normally sees.
  constexpr Real min_valid_temperature = 280.0; // K - just above freezing; below this a
                                                // liquid-water fp correlation is out of its
                                                // valid domain and cannot be trusted not to
                                                // return garbage
  constexpr Real max_valid_temperature = 600.0; // K - ~100K above this well's hottest IC (503K),
                                                // ample margin for a Newton excursion without
                                                // straying near the correlation's own limits
  constexpr Real min_valid_cp = 100.0;           // J/(kg.K) - well below water's cp anywhere sane
  constexpr Real max_ntu = 50.0;                 // exp(-50) is already indistinguishable from 0

  Real t_avg_numerator = 0.0;
  Real t_avg_length = 0.0;
  for (int i = i_start; i >= 0; --i)
  {
    const Real length = 2.0 * _half_seg_len[i];
    t_avg_numerator += length * 0.5 * (temperature_f[i] + temperature_f[i + 1]);
    t_avg_length += length;
  }
  const Real t_avg = t_avg_numerator / t_avg_length;

  const Real t_for_cp = std::min(std::max(t_entry, min_valid_temperature), max_valid_temperature);
  const Real cp = std::max(_fp.cp_from_p_T(_reference_pressure, t_for_cp), min_valid_cp);

  if (_flow_direction == FlowDirection::production)
  {
    // Fluid enters at the open/cased boundary (node i_start+1, which is NOT one of this
    // kernel's own active points) and s increases toward the wellhead (index 0).
    const Real perimeter_avg = libMesh::pi * (_weight->at(i_start) + _weight->at(i_start + 1));
    Real cumulative_length = 0.0;
    for (int i = i_start; i >= 0; --i)
    {
      cumulative_length += 2.0 * _half_seg_len[i];
      const Real ntu = std::min(_h * perimeter_avg * cumulative_length / (mdot * cp), max_ntu);
      _t_well[i] = t_avg + (t_entry - t_avg) * std::exp(-ntu);
    }
  }
  else
  {
    // Injection: fluid enters at the wellhead (index 0, one of this kernel's own active points)
    // and s increases toward the open/cased boundary. Because the wellhead IS the entry point
    // (unlike production's entry point, which sits just outside this kernel's own active range),
    // cumulative_length starts at exactly 0, so _t_well[0] equals injection_temperature exactly -
    // not one segment's worth of decay in, the way production's first active point is.
    const Real perimeter_avg = libMesh::pi * (_weight->at(0) + _weight->at(1));
    Real cumulative_length = 0.0;
    for (int i = 0; i <= i_start; ++i)
    {
      if (i > 0)
        cumulative_length += 2.0 * _half_seg_len[i - 1];
      const Real ntu = std::min(_h * perimeter_avg * cumulative_length / (mdot * cp), max_ntu);
      _t_well[i] = t_avg + (t_entry - t_avg) * std::exp(-ntu);
    }
  }
}

Real
PorousFlowCasedBoreholeHeatExchange::computeQpResidual()
{
  const unsigned current_dirac_ptid = currentPointCachedID();
  const Real character = _character.value(_t, _q_point[_qp]);
  if (character == 0.0)
    return 0.0;

  const std::size_t num_pts = _z_coord->size();
  Real length = 0.0;
  if (current_dirac_ptid > 0)
    length += _half_seg_len[current_dirac_ptid - 1];
  if (current_dirac_ptid + 1 < num_pts || num_pts == 1)
    length += _half_seg_len[current_dirac_ptid];

  const Real perimeter = 2.0 * libMesh::pi * _weight->at(current_dirac_ptid);
  const Real outflow = _h * perimeter * length * (_u[_qp] - _t_well[current_dirac_ptid]) *
                       _test[_i][_qp] * std::abs(character);

  // This is not thread safe, but DiracKernels aren't currently threaded - same convention as
  // PorousFlowLineSink::computeQpResidual().
  _total_outflow.add(outflow * _dt);
  _point_fluxes.add(current_dirac_ptid, outflow);

  return outflow;
}

Real
PorousFlowCasedBoreholeHeatExchange::computeQpJacobian()
{
  const unsigned current_dirac_ptid = currentPointCachedID();
  const Real character = _character.value(_t, _q_point[_qp]);
  if (character == 0.0)
    return 0.0;

  const std::size_t num_pts = _z_coord->size();
  Real length = 0.0;
  if (current_dirac_ptid > 0)
    length += _half_seg_len[current_dirac_ptid - 1];
  if (current_dirac_ptid + 1 < num_pts || num_pts == 1)
    length += _half_seg_len[current_dirac_ptid];

  const Real perimeter = 2.0 * libMesh::pi * _weight->at(current_dirac_ptid);

  // _t_well is a true constant throughout this time step's Newton solve (see
  // computeWellboreTemperatures()): it depends only on the *old* solution and the lagged
  // mass_point_flux_vpp, never on the current iterate. The only remaining dependence of the
  // residual on the current iterate is _u[_qp] itself, at this same point, which is exactly
  // what this Jacobian differentiates.
  return _h * perimeter * length * _phi[_j][_qp] * _test[_i][_qp] * std::abs(character);
}
