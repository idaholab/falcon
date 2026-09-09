#!/usr/bin/env python3
"""Generate a schematic diagram of the geothermal_wellbore example's simulation domain, on a
radial slice (mirroring moose's ATES_geometry.png for its own axisymmetric example).

Usage: python3 make_schematic.py
"""
import matplotlib.pyplot as plt
import matplotlib.patches as patches

R_MAX = 1000.0
Y_SURFACE = 0.0
Y_INTERFACE = -1000.0  # cap/reservoir boundary, also the well's cased/open boundary
Y_BOTTOM = -2000.0


def main():
    fig, ax = plt.subplots(figsize=(7.5, 6.2))

    # Layers
    ax.add_patch(patches.Rectangle((0, Y_INTERFACE), R_MAX, Y_SURFACE - Y_INTERFACE,
                                    facecolor="#d8c9a3", edgecolor="none", zorder=0))
    ax.add_patch(patches.Rectangle((0, Y_BOTTOM), R_MAX, Y_INTERFACE - Y_BOTTOM,
                                    facecolor="#a3c4d8", edgecolor="none", zorder=0))
    ax.text(R_MAX * 0.38, (Y_SURFACE + Y_INTERFACE) / 2,
            "cap\n(low permeability,\nexaggerated thermal response)",
            ha="center", va="center", fontsize=10)
    ax.text(R_MAX * 0.5, Y_INTERFACE - 150, "reservoir\n(normal geologic media)",
            ha="center", va="center", fontsize=10)

    # Well, drawn as a bold line at r=0 (true radius 0.1m, far too thin to draw to scale) -
    # dashed/grey for the cased section, solid/dark for the open (perforated) section.
    ax.plot([0, 0], [Y_SURFACE, Y_INTERFACE], color="#777777", linewidth=6,
            linestyle=(0, (3, 2)), solid_capstyle="butt", zorder=2)
    ax.plot([0, 0], [Y_INTERFACE, Y_BOTTOM], color="#222222", linewidth=6,
            solid_capstyle="butt", zorder=2)

    ax.annotate("cased\n(no flow/heat exchange)", xy=(0, -500), xytext=(R_MAX * 0.30, -300),
                fontsize=9, style="italic", ha="left", va="center",
                arrowprops=dict(arrowstyle="-", color="#777777", lw=0.8))
    ax.annotate("open interval\n(mass + heat exchange,\n403-503 K)", xy=(0, -1700),
                xytext=(R_MAX * 0.30, -1850), fontsize=9, style="italic", ha="left", va="center",
                arrowprops=dict(arrowstyle="-", color="#222222", lw=0.8))

    # Interface line
    ax.axhline(Y_INTERFACE, color="black", linewidth=0.8, linestyle="--", zorder=1)
    ax.text(R_MAX * 0.98, Y_INTERFACE - 45, "y = -1000 m", fontsize=8, color="black", ha="right")

    # Outer recharge boundary
    ax.axvline(R_MAX, color="#a85a2a", linewidth=2.5, zorder=1)
    ax.text(R_MAX - 20, Y_BOTTOM + 60, "recharge\nboundary\n(fixed P, T)",
            ha="right", va="bottom", fontsize=9, color="#a85a2a")

    # Symmetry axis label
    ax.text(20, Y_SURFACE - 30, "r = 0\n(symmetry axis)", fontsize=8, va="top", ha="left")

    # Geothermal gradient annotation - offset well clear of the interface line and well labels
    grad_x = R_MAX * 0.78
    ax.annotate("", xy=(grad_x, Y_BOTTOM + 100), xytext=(grad_x, Y_SURFACE - 100),
                arrowprops=dict(arrowstyle="->", color="#0b6f6b"))
    ax.text(grad_x + 25, (Y_SURFACE + Y_BOTTOM) / 2 + 400,
            "geothermal gradient:\n30 degC at surface,\n100 degC/km",
            fontsize=8.5, color="#0b6f6b", va="center")

    ax.set_xlim(0, R_MAX * 1.02)
    ax.set_ylim(Y_BOTTOM * 1.03, 60)
    ax.set_xlabel("radius, r (m)")
    ax.set_ylabel("depth, y (m)")
    ax.set_title("Geometry on a radial (RZ) slice of the geothermal wellbore model")
    fig.tight_layout()
    fig.savefig("schematic.png", dpi=150)
    print("Saved schematic.png")


if __name__ == "__main__":
    main()
