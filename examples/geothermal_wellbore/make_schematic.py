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

# The 11 Peaceman points where withdraw_fluid/withdraw_heat are active (well_character=1,
# geothermal_wellbore.bh points y=-1000 to y=-2000 in 100m steps) - the only points where fluid
# actually crosses from the formation into the well.
OPEN_POINTS = [Y_INTERFACE - 100.0 * i for i in range(11)]


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
    # solid where the casing seals the well off from the formation (no mass can cross; only
    # PorousFlowCasedBoreholeHeatExchange's conductive heat exchange applies), dashed where the
    # well is open to the formation and fluid can actually cross into it. Dashes are drawn as
    # explicit short segments in data coordinates (one dash+gap per 100m Peaceman spacing)
    # rather than via a point-based linestyle, which doesn't read clearly at this linewidth.
    ax.plot([0, 0], [Y_SURFACE, Y_INTERFACE], color="#777777", linewidth=6,
            solid_capstyle="butt", zorder=2)
    dash_len = 60.0  # of each 100m point-to-point segment; the rest is the gap
    for y0 in OPEN_POINTS[:-1]:
        ax.plot([0, 0], [y0, y0 - dash_len], color="#222222", linewidth=6,
                solid_capstyle="butt", zorder=2)

    # Peaceman points along the open interval - the discrete locations where mass (and, via the
    # enthalpy it carries, heat) actually leaves the formation and enters the well.
    ax.scatter([0] * len(OPEN_POINTS), OPEN_POINTS, s=55, facecolor="white",
               edgecolor="#222222", linewidth=1.3, zorder=3)

    ax.annotate("cased\n(sealed to mass flow;\nconductive heat exchange only)",
                xy=(0, -500), xytext=(R_MAX * 0.30, -300),
                fontsize=9, style="italic", ha="left", va="center",
                arrowprops=dict(arrowstyle="-", color="#777777", lw=0.8))
    ax.annotate("open interval\n(mass + advective heat exchange,\n403-503 K)", xy=(0, -1700),
                xytext=(R_MAX * 0.30, -1850), fontsize=9, style="italic", ha="left", va="center",
                arrowprops=dict(arrowstyle="-", color="#222222", lw=0.8))
    ax.annotate("Peaceman points:\nfluid leaves the domain\nhere, one every 100m",
                xy=(0, OPEN_POINTS[3]), xytext=(-R_MAX * 0.42, OPEN_POINTS[3] + 120),
                fontsize=9, style="italic", ha="left", va="center",
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

    ax.set_xlim(-R_MAX * 0.46, R_MAX * 1.02)
    ax.set_ylim(Y_BOTTOM * 1.03, 60)
    ax.set_xlabel("radius, r (m)")
    ax.set_ylabel("depth, y (m)")
    ax.set_title("Geometry on a radial (RZ) slice of the geothermal wellbore model")
    fig.tight_layout()
    fig.savefig("schematic.png", dpi=150)
    print("Saved schematic.png")


if __name__ == "__main__":
    main()
