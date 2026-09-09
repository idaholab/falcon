#!/usr/bin/env python3
"""Compare the old (constant unit_weight) and new (unit_weight_fp) PorousFlowPeacemanBorehole
wellbore-pressure treatments for this example: extracted mass/heat rate over time, pressure vs.
depth along the well, temperature vs. time at a series of depths, and the temperature halo at
the cap/reservoir interface (y=-1000, right at the well's open interval).

Usage: python3 plot_results.py
Run from this directory after both variants have been run with falcon-opt:
  falcon-opt -i production.i
  falcon-opt -i production_constant_unit_weight.i
"""
import glob

import matplotlib.pyplot as plt
import pandas as pd

NEW = "production"
OLD = "production_constant_unit_weight"
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


def main():
    old, new = load_main(OLD), load_main(NEW)

    cum_mass_old, cum_mass_new = old["well_mass_rate"].sum(), new["well_mass_rate"].sum()
    cum_heat_old, cum_heat_new = old["well_heat_rate"].sum(), new["well_heat_rate"].sum()

    print("Cumulative extracted mass (kg): old=%.4g new=%.4g (%.1f%% difference)" %
          (cum_mass_old, cum_mass_new, 100 * (cum_mass_new - cum_mass_old) / cum_mass_old))
    print("Cumulative extracted heat (J):  old=%.4g new=%.4g (%.1f%% difference)" %
          (cum_heat_old, cum_heat_new, 100 * (cum_heat_new - cum_heat_old) / cum_heat_old))

    # 1. Extracted mass/heat rate vs time
    fig, axes = plt.subplots(2, 1, figsize=(7, 8), sharex=True)
    axes[0].plot(old["time"] / YEAR, old["well_mass_rate"], "o-", label="constant unit_weight")
    axes[0].plot(new["time"] / YEAR, new["well_mass_rate"], "s-", label="unit_weight_fp")
    axes[0].set_ylabel("mass extracted per step (kg)")
    axes[0].legend()
    axes[0].set_title("Extracted mass per timestep")

    axes[1].plot(old["time"] / YEAR, old["well_heat_rate"], "o-", label="constant unit_weight")
    axes[1].plot(new["time"] / YEAR, new["well_heat_rate"], "s-", label="unit_weight_fp")
    axes[1].set_ylabel("heat extracted per step (J)")
    axes[1].set_xlabel("time (years)")
    axes[1].legend()
    axes[1].set_title("Extracted heat per timestep")
    fig.tight_layout()
    fig.savefig("rates_vs_time.png", dpi=150)
    print("Saved rates_vs_time.png")

    # 2. Pressure vs. depth along the well axis, final step - old vs. new, plus the
    # difference (new - old) alongside, since the two profiles are close enough in absolute
    # terms that the difference isn't visible on a shared axis.
    old_p = load_final_line(OLD, "porepressure_along_well_axis")
    new_p = load_final_line(NEW, "porepressure_along_well_axis")
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(11, 7), sharey=True)
    ax1.plot(old_p["porepressure"] / 1e6, old_p["y"], "o-", label="constant unit_weight",
             markersize=3)
    ax1.plot(new_p["porepressure"] / 1e6, new_p["y"], "s-", label="unit_weight_fp", markersize=3)
    ax1.axhline(-1000, color="gray", linewidth=0.8, linestyle="--")
    ax1.set_xlabel("porepressure (MPa)")
    ax1.set_ylabel("depth, y (m)")
    ax1.set_title("Pressure vs. depth, final step")
    ax1.legend()

    diff_kpa = (new_p["porepressure"].to_numpy() - old_p["porepressure"].to_numpy()) / 1e3
    ax2.plot(diff_kpa, new_p["y"], color="#a85a2a")
    ax2.axvline(0, color="black", linewidth=0.6)
    ax2.axhline(-1000, color="gray", linewidth=0.8, linestyle="--")
    ax2.text(ax2.get_xlim()[1], -1000, " cap/reservoir\n interface", fontsize=8, color="gray",
             va="center")
    ax2.set_xlabel("unit_weight_fp minus constant (kPa)")
    ax2.set_title("Difference")
    fig.tight_layout()
    fig.savefig("pressure_depth.png", dpi=150)
    print("Saved pressure_depth.png")

    # 3. Temperature CHANGE vs. time at a series of depths (new variant). Plotted relative to
    # each depth's own initial value, not absolute temperature - the geothermal gradient means
    # absolute temperatures span 300-500K between depths, which would swamp the actual dynamics
    # (a few K at most) on a shared axis. The spurious time=0 row (postprocessors report 0
    # before the first solve) is dropped.
    new_t = new[new["time"] > 0]
    fig, ax = plt.subplots(figsize=(8, 5.5))
    depths = sorted(DEPTH_POSTPROCESSORS, reverse=True)
    cmap = plt.get_cmap("viridis")
    for i, depth in enumerate(depths):
        col = DEPTH_POSTPROCESSORS[depth]
        delta = new_t[col] - new_t[col].iloc[0]
        ax.plot(new_t["time"] / YEAR, delta, color=cmap(i / (len(depths) - 1)),
                label=f"y = {depth} m")
    ax.axhline(0, color="black", linewidth=0.6)
    ax.set_xlabel("time (years)")
    ax.set_ylabel("temperature change from t~0 (K)")
    ax.set_title("Temperature change vs. time at depths along the well axis (unit_weight_fp)")
    ax.legend(fontsize=8, ncol=2)
    fig.tight_layout()
    fig.savefig("temperature_vs_time_depths.png", dpi=150)
    print("Saved temperature_vs_time_depths.png")

    # 4. Temperature halo at the cap/reservoir interface, final step
    old_halo = load_final_line(OLD, "temperature_halo")
    new_halo = load_final_line(NEW, "temperature_halo")
    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(old_halo["x"], old_halo["temperature"], "o-", label="constant unit_weight")
    ax.plot(new_halo["x"], new_halo["temperature"], "s-", label="unit_weight_fp")
    ax.set_xlabel("radius, r (m)")
    ax.set_ylabel("temperature (K)")
    ax.set_title("Temperature halo at the cap/reservoir interface (y=-1000), final step")
    ax.legend()
    fig.tight_layout()
    fig.savefig("temperature_halo.png", dpi=150)
    print("Saved temperature_halo.png")


if __name__ == "__main__":
    main()
