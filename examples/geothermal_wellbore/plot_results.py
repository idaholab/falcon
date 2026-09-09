#!/usr/bin/env python3
"""Compare the old (constant unit_weight) and new (unit_weight_fp) PorousFlowPeacemanBorehole
wellbore-pressure treatments for this example: extracted mass/heat rate over time, and the
temperature halo at the cap/reservoir interface (y=-1000, right at the well's open interval).

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


def load_main(prefix):
    return pd.read_csv(f"{prefix}_out.csv")


def load_final_halo(prefix):
    files = sorted(glob.glob(f"{prefix}_out_temperature_halo_*.csv"))
    return pd.read_csv(files[-1])


def main():
    old, new = load_main(OLD), load_main(NEW)

    old_final, new_final = old.iloc[-1], new.iloc[-1]
    cum_mass_old, cum_mass_new = old["well_mass_rate"].sum(), new["well_mass_rate"].sum()
    cum_heat_old, cum_heat_new = old["well_heat_rate"].sum(), new["well_heat_rate"].sum()

    print("Cumulative extracted mass (kg): old=%.4g new=%.4g (%.1f%% difference)" %
          (cum_mass_old, cum_mass_new, 100 * (cum_mass_new - cum_mass_old) / cum_mass_old))
    print("Cumulative extracted heat (J):  old=%.4g new=%.4g (%.1f%% difference)" %
          (cum_heat_old, cum_heat_new, 100 * (cum_heat_new - cum_heat_old) / cum_heat_old))

    # Extracted mass/heat rate vs time
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

    # Temperature halo at the cap/reservoir interface, final step
    old_halo, new_halo = load_final_halo(OLD), load_final_halo(NEW)
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
