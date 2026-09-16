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
      "The name of a PorousFlowPlotPointFluxQuantity VectorPostprocessor reporting the produced "
      "mass flux (its 'flux' vector) at each point of the open/perforated section, using the "
      "same point_file as this kernel. Used both to get the total produced mass rate and to "
      "compute the mass-flux-weighted mixing temperature of the fluid entering the cased "
      "section - the fluid produced deeper in the open interval is hotter, so a mixing average "
      "is used rather than the local formation temperature at the open/cased boundary alone.");
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
    _mass_flux(getVectorPostprocessorValue("mass_point_flux_vpp", "flux")),
    _fp(getUserObject<SinglePhaseFluidProperties>("wellbore_fp")),
    _reference_pressure(getParam<Real>("wellbore_reference_pressure")),
    _temperature_system(_var.sys().system()),
    _temperature_var_number(_var.number()),
    _total_outflow(
        const_cast<PorousFlowSumQuantity &>(getUserObject<PorousFlowSumQuantity>("SumQuantityUO"))),
    _point_fluxes(const_cast<PorousFlowPointFluxQuantity &>(
        getUserObject<PorousFlowPointFluxQuantity>("PointFluxUO")))
{
}

void
PorousFlowCasedBoreholeHeatExchange::initialSetup()
{
  PorousFlowLineGeometry::initialSetup();

  if (_z_coord->size() < 2)
    mooseError("PorousFlowCasedBoreholeHeatExchange: at least two well points are required to "
               "march the wellbore-fluid-temperature profile along a segment.");
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

  // mass_point_flux_vpp's underlying vector is cleared to size 0 by its own initialize() and
  // only refilled by its own execute(), on its own schedule - before its first execute() (eg
  // going into the very first real solve), or transiently between initialize() and execute(),
  // it can be empty. Treat that exactly like "no produced flow yet".
  if (_mass_flux.size() != num_pts)
    return;

  // Total produced mass rate and the mass-flux-weighted mixing temperature of everything
  // produced across the open interval - the fluid entering deeper in that interval is hotter,
  // so this mixing average (not the local formation temperature at the open/cased boundary
  // alone) is the physically correct temperature of the fluid entering the cased section.
  Real mdot = 0.0;
  Real mdot_t = 0.0;
  for (const auto i : make_range(num_pts))
  {
    mdot += _mass_flux[i];
    mdot_t += _mass_flux[i] * temperature_f[i];
  }
  if (mdot <= 0.0)
    return; // no produced flow yet (eg the first time step, before mass_point_flux_vpp has run)

  const Real t_mix = mdot_t / mdot;

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
  // open/cased boundary point i_start+1 that t_mix enters at), and the fluid's specific heat
  // evaluated once at the entry temperature - both single, well-mixed values rather than a
  // segment-by-segment recomputation, so T_well at any point depends only on t_mix and this
  // point's own cumulative distance from the boundary, never on another cased point's T_well.
  //
  // temperature_f, t_mix, and t_avg are all sampled from the *old* (previous time step's
  // converged) solution (see above), so none of this is reachable with an unphysical
  // intermediate value from the current Newton iterate. It is still clamped defensively: the
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

  const Real t_for_cp = std::min(std::max(t_mix, min_valid_temperature), max_valid_temperature);
  const Real cp = std::max(_fp.cp_from_p_T(_reference_pressure, t_for_cp), min_valid_cp);
  const Real perimeter_avg = libMesh::pi * (_weight->at(i_start) + _weight->at(i_start + 1));

  Real cumulative_length = 0.0;
  for (int i = i_start; i >= 0; --i)
  {
    cumulative_length += 2.0 * _half_seg_len[i];
    const Real ntu = std::min(_h * perimeter_avg * cumulative_length / (mdot * cp), max_ntu);
    _t_well[i] = t_avg + (t_mix - t_avg) * std::exp(-ntu);
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
