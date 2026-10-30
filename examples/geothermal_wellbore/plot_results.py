#!/usr/bin/env python3
"""Compare the old (constant unit_weight) and new (unit_weight_fp) PorousFlowPeacemanBorehole
wellbore-pressure treatments for the production example: extracted mass/heat rate over time,
pressure vs. depth along the well's open (feed) interval, and temperature vs. time at a series
of depths. Also plots the rate-controlled injection example: delivered rate/bottomhole pressure
vs. time (showing the secant correction converge), and per-point injected mass flux vs. depth
(showing the flow partition itself by local permeability across the high-k sub-zone).

Usage: python3 plot_results.py
Run from this directory after all three variants have been run with falcon-opt:
  falcon-opt -i production.i
  falcon-opt -i production_constant_unit_weight.i
  falcon-opt -i injection.i
"""
import glob

import matplotlib.pyplot as plt
import pandas as pd

NEW = "production"
OLD = "production_constant_unit_weight"
INJ = "injection"
YEAR = 3.15576e7

# Depth (m) -> main-CSV postprocessor name, for the temperature-vs-time-at-depth plot.
DEPTH_POSTPROCESSORS = {
    0: "T_wellhead",
    -200: "T_y200",
    -400: "T_y400",
    -600: "T_y600",
    -800: "T_y800",
    -1000: "T_open_top",
    -1500: "T_y1500",
    -2000: "T_bottomhole_formation",
}


def load_main(prefix):
    return pd.read_csv(f"{prefix}_out.csv")


def load_final_line(prefix, sampler):
    files = sorted(glob.glob(f"{prefix}_out_{sampler}_*.csv"))
    return pd.read_csv(files[-1])


def load_lines_near_times(prefix, sampler, target_times):
    """Load the sampler output file closest to each target time (seconds), skipping index 0
    (this model's VectorPostprocessors report empty output at t=0). File index N is assumed to
    align with row N of the main CSV (both advance one-for-one with accepted timesteps), which
    holds as long as the run has no cut/retried timesteps."""
    files = sorted(glob.glob(f"{prefix}_out_{sampler}_*.csv"))[1:]
    times = load_main(prefix)["time"].to_numpy()[1:]
    selected = []
    for target in target_times:
        idx = int(abs(times - target).argmin())
        selected.append((times[idx], pd.read_csv(files[idx])))
    return selected


def main():
    old, new = load_main(OLD), load_main(NEW)

    cum_mass_old, cum_mass_new = old["well_mass_rate"].sum(), new["well_mass_rate"].sum()
    cum_heat_old, cum_heat_new = old["well_heat_rate"].sum(), new["well_heat_rate"].sum()

    print("Cumulative extracted mass (kg): old=%.4g new=%.4g (%.1f%% difference)" %
          (cum_mass_old, cum_mass_new, 100 * (cum_mass_new - cum_mass_old) / cum_mass_old))
    print("Cumulative extracted heat (J):  old=%.4g new=%.4g (%.1f%% difference)" %
          (cum_heat_old, cum_heat_new, 100 * (cum_heat_new - cum_heat_old) / cum_heat_old))

    # 1. Extracted mass/heat RATE vs time - true rates (value/dt), not the raw per-timestep
    # accumulated quantities well_mass_rate/well_heat_rate actually are (see
    # PorousFlowRateControlledBoreholePressure.md's own "units trap" note - the same trap applies
    # here). Plotting the raw accumulated values directly against time produces a misleading
    # drop at the very end of the run: IterationAdaptiveDT's last step is clipped short to land
    # exactly on end_time, so the accumulated-per-step quantity there is smaller even though the
    # true underlying rate is not - dividing by each row's own dt removes that artifact.
    old_dt, new_dt = old["time"].diff(), new["time"].diff()
    fig, axes = plt.subplots(2, 1, figsize=(7, 8), sharex=True)
    axes[0].plot(old["time"] / YEAR, old["well_mass_rate"] / old_dt, "o-",
                 label="constant unit_weight")
    axes[0].plot(new["time"] / YEAR, new["well_mass_rate"] / new_dt, "s-", label="unit_weight_fp")
    axes[0].set_ylabel("mass extraction rate (kg/s)")
    axes[0].legend()
    axes[0].set_title("Extracted mass rate")

    axes[1].plot(old["time"] / YEAR, old["well_heat_rate"] / old_dt, "o-",
                 label="constant unit_weight")
    axes[1].plot(new["time"] / YEAR, new["well_heat_rate"] / new_dt, "s-", label="unit_weight_fp")
    axes[1].set_ylabel("heat extraction rate (W)")
    axes[1].set_xlabel("time (years)")
    axes[1].legend()
    axes[1].set_title("Extracted heat rate")
    fig.tight_layout()
    fig.savefig("rates_vs_time.png", dpi=150)
    print("Saved rates_vs_time.png")

    # 2. Pressure vs. depth, final step - old vs. new, plus the difference (new - old)
    # alongside, since the two profiles are close enough in absolute terms that the difference
    # isn't visible on a shared axis. Restricted to the open (feed) interval, y=-1000 to -2000 -
    # the cased section above it carries no mass at all in either treatment, so it adds nothing
    # to this particular comparison.
    old_p = load_final_line(OLD, "porepressure_along_well_axis")
    new_p = load_final_line(NEW, "porepressure_along_well_axis")
    old_p = old_p[old_p["y"] <= -1000]
    new_p = new_p[new_p["y"] <= -1000]

    bottom_pp_old = old_p[old_p["y"] == -2000]["porepressure"].iloc[0]
    bottom_pp_new = new_p[new_p["y"] == -2000]["porepressure"].iloc[0]
    print("Pressure at well bottom (y=-2000): old=%.4g MPa new=%.4g MPa (bottomhole_pressure "
          "Function itself = 18.02 MPa - PorousFlowPeacemanBorehole's Peaceman coupling is a "
          "finite-well-index source term, not a hard Dirichlet constraint, so the solved field "
          "converges close to, not exactly onto, that imposed value)" %
          (bottom_pp_old / 1e6, bottom_pp_new / 1e6))

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(11, 7), sharey=True)
    ax1.plot(old_p["porepressure"] / 1e6, old_p["y"], "o-", label="constant unit_weight",
             markersize=4)
    ax1.plot(new_p["porepressure"] / 1e6, new_p["y"], "s-", label="unit_weight_fp", markersize=4)
    ax1.set_xlabel("porepressure (MPa)")
    ax1.set_ylabel("depth, y (m)")
    ax1.set_title("Pressure vs. depth, open interval, final step")
    ax1.legend()

    diff_kpa = (new_p["porepressure"].to_numpy() - old_p["porepressure"].to_numpy()) / 1e3
    ax2.plot(diff_kpa, new_p["y"], color="#a85a2a")
    ax2.axvline(0, color="black", linewidth=0.6)
    ax2.set_xlabel("unit_weight_fp minus constant (kPa)")
    ax2.set_title("Difference")
    fig.tight_layout()
    fig.savefig("pressure_depth.png", dpi=150)
    print("Saved pressure_depth.png")

    # 3. Absolute temperature vs. time at a series of depths (new variant). The spurious time=0
    # row (postprocessors report 0 before the first solve) is dropped.
    new_t = new[new["time"] > 0]
    fig, ax = plt.subplots(figsize=(8, 5.5))
    depths = sorted(DEPTH_POSTPROCESSORS, reverse=True)
    cmap = plt.get_cmap("viridis")
    for i, depth in enumerate(depths):
        col = DEPTH_POSTPROCESSORS[depth]
        ax.plot(new_t["time"] / YEAR, new_t[col], color=cmap(i / (len(depths) - 1)),
                label=f"y = {depth} m")
    ax.set_xlabel("time (years)")
    ax.set_ylabel("temperature (K)")
    ax.set_title("Temperature vs. time at depths along the well axis (unit_weight_fp)")
    ax.legend(fontsize=8, ncol=2)
    fig.tight_layout()
    fig.savefig("temperature_vs_time_depths.png", dpi=150)
    print("Saved temperature_vs_time_depths.png")

    # 4. Temperature vs. radius at mid-cap depth (y=-500), at a handful of times through the
    # run - shows the radial thermal signal developing inside the cap itself (unit_weight_fp
    # variant only, since the point is the cased-section heat exchange's own effect, not the
    # pressure-treatment comparison from plots 1-2). Zoomed to the nearest 50m: the whole signal
    # decays to background within about that distance (see model_common.i's own
    # cap_temperature_radial sampler, which sits at 2m spacing over the nearest 100m
    # specifically so this zoom has enough points to look smooth, not just cropped). The
    # cap/reservoir-interface halo this same radial signature would show at y=-1000 is not
    # plotted separately - it is materially the same story at that depth too, so a second,
    # near-identical figure would add a comparison without adding information.
    target_times = [i * YEAR for i in range(6)]
    snapshots = load_lines_near_times(NEW, "cap_temperature_radial", target_times)
    fig, ax = plt.subplots(figsize=(7, 5))
    cmap = plt.get_cmap("viridis")
    for i, (t, df) in enumerate(snapshots):
        ax.plot(df["x"], df["temperature"], color=cmap(i / (len(snapshots) - 1)),
                label=f"t = {t / YEAR:.1f} yr")
    ax.set_xlim(0, 50)
    ax.set_xlabel("radius, r (m)")
    ax.set_ylabel("temperature (K)")
    ax.set_title("Temperature vs. radius at mid-cap depth (y=-500), unit_weight_fp")
    ax.legend(fontsize=8)
    fig.tight_layout()
    fig.savefig("cap_temperature_radial.png", dpi=150)
    print("Saved cap_temperature_radial.png")

    plot_injection()


def plot_injection():
    inj = load_main(INJ)
    inj = inj[inj["time"] > 0]

    target = -20.0  # kg/s - see wells_injection.i's target_injection_rate

    # 6. Delivered rate and controlled bottomhole pressure vs. time - the direct evidence the
    # secant correction converges on the target rate rather than merely running at some
    # arbitrary fixed pressure.
    fig, ax1 = plt.subplots(figsize=(8, 5.5))
    ax2 = ax1.twinx()
    ax1.plot(inj["time"] / YEAR, inj["injected_mass_rate"], "s-", color="#1f77b4",
             label="delivered rate")
    ax1.axhline(target, color="#1f77b4", linewidth=0.8, linestyle="--", label="target rate")
    ax2.plot(inj["time"] / YEAR, inj["bhp_control"] / 1e6, "o-", color="#a85a2a",
             label="bottomhole pressure")
    ax1.set_xlabel("time (years)")
    ax1.set_ylabel("injected mass rate (kg/s)", color="#1f77b4")
    ax2.set_ylabel("bhp_control (MPa)", color="#a85a2a")
    ax1.tick_params(axis="y", labelcolor="#1f77b4")
    ax2.tick_params(axis="y", labelcolor="#a85a2a")
    ax1.set_title("Rate-controlled injection: delivered rate and bottomhole pressure")
    fig.legend(loc="lower right")
    fig.tight_layout()
    fig.savefig("injection_rate_and_bhp.png", dpi=150)
    print("Saved injection_rate_and_bhp.png")

    # 7. Per-point injected mass flux vs. depth, final step - the direct evidence that flow
    # partitions itself by local permeability: the three points inside the high-k sub-zone
    # (y=-1400 to -1600, injection_zones.i) should carry visibly more flux than their
    # uniform-permeability neighbors, with NOTHING in the model explicitly imposing that split.
    flux = load_final_line(INJ, "mass_point_flux")
    fig, ax = plt.subplots(figsize=(6, 7))
    ax.plot(flux["flux"], flux["y"], "o-", markersize=4)
    ax.axhspan(-1600, -1400, color="#d62728", alpha=0.15, label="high-k sub-zone (10x)")
    ax.set_xlabel("injected mass flux (kg/s)")
    ax.set_ylabel("depth, y (m)")
    ax.set_title("Injected mass flux vs. depth, final step")
    ax.legend()
    fig.tight_layout()
    fig.savefig("injection_flux_vs_depth.png", dpi=150)
    print("Saved injection_flux_vs_depth.png")


if __name__ == "__main__":
    main()
