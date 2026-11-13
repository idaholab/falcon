#!/usr/bin/env python3
"""Generate schematic diagrams of the geothermal_wellbore example's simulation domain, on a
radial slice (mirroring moose's ATES_geometry.png for its own axisymmetric example): one for the
production well, one for the rate-controlled injection well - the same geometry, but the latter
also marks injection_zones.i's higher-permeability sub-zone and the direction of flow.

Usage: python3 make_schematic.py
"""
import matplotlib.pyplot as plt
import matplotlib.patches as patches

R_MAX = 1000.0
Y_SURFACE = 0.0
Y_INTERFACE = -1000.0  # cap/reservoir boundary, also the well's cased/open boundary
Y_BOTTOM = -2000.0

# The 11 Peaceman points where withdraw_fluid/withdraw_heat (production) or inject_fluid/
# inject_heat (injection) are active (character != 0, geothermal_wellbore.bh points y=-1000 to
# y=-2000 in 100m steps) - the only points where fluid actually crosses between the formation and
# the well.
OPEN_POINTS = [Y_INTERFACE - 100.0 * i for i in range(11)]

# injection_zones.i's higher-permeability sub-zone (10x the surrounding reservoir), entirely
# within the open interval.
HIGH_K_TOP = -1400.0
HIGH_K_BOTTOM = -1600.0


def draw_base_geometry(ax, open_interval_label, reservoir_label_y=Y_INTERFACE - 150):
    """Draws everything common to both wells: the cap/reservoir layers, the well line
    (solid=cased, dashed=open, with the 11 Peaceman points marked), the cap/reservoir interface,
    the recharge boundary, and the geothermal gradient annotation. open_interval_label lets the
    two callers describe what happens at those Peaceman points differently (fluid leaving vs.
    entering the formation); reservoir_label_y lets the injection schematic move this label out
    of the way of its own high-permeability-zone annotation."""
    # Layers
    ax.add_patch(patches.Rectangle((0, Y_INTERFACE), R_MAX, Y_SURFACE - Y_INTERFACE,
                                    facecolor="#d8c9a3", edgecolor="none", zorder=0))
    ax.add_patch(patches.Rectangle((0, Y_BOTTOM), R_MAX, Y_INTERFACE - Y_BOTTOM,
                                    facecolor="#a3c4d8", edgecolor="none", zorder=0))
    ax.text(R_MAX * 0.38, (Y_SURFACE + Y_INTERFACE) / 2,
            "cap\n(realistic properties:\nk=1E-16 m^2, k_th=2.5 W/m/K)",
            ha="center", va="center", fontsize=10)
    ax.text(R_MAX * 0.5, reservoir_label_y, "reservoir\n(normal geologic media)",
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

    # Peaceman points along the open interval.
    ax.scatter([0] * len(OPEN_POINTS), OPEN_POINTS, s=55, facecolor="white",
               edgecolor="#222222", linewidth=1.3, zorder=3)

    ax.annotate("cased\n(sealed to mass flow;\nconductive heat exchange only)",
                xy=(0, -500), xytext=(R_MAX * 0.30, -300),
                fontsize=9, style="italic", ha="left", va="center",
                arrowprops=dict(arrowstyle="-", color="#777777", lw=0.8))
    ax.annotate(open_interval_label, xy=(0, -1700),
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

    ax.set_xlim(-R_MAX * 0.46, R_MAX * 1.02)
    ax.set_ylim(Y_BOTTOM * 1.03, 60)
    ax.set_xlabel("radius, r (m)")
    ax.set_ylabel("depth, y (m)")


def make_production_schematic():
    fig, ax = plt.subplots(figsize=(7.5, 6.2))
    draw_base_geometry(ax, "open interval\n(mass + advective heat exchange,\n403-503 K)")
    ax.annotate("Peaceman points:\nfluid leaves the domain\nhere, one every 100m",
                xy=(0, OPEN_POINTS[3]), xytext=(-R_MAX * 0.42, OPEN_POINTS[3] + 120),
                fontsize=9, style="italic", ha="left", va="center",
                arrowprops=dict(arrowstyle="-", color="#222222", lw=0.8))
    ax.set_title("Geometry on a radial (RZ) slice of the geothermal wellbore model")
    fig.tight_layout()
    fig.savefig("schematic.png", dpi=150)
    print("Saved schematic.png")


def make_injection_schematic():
    fig, ax = plt.subplots(figsize=(7.5, 6.2))
    # Moved up, right under the cap/reservoir interface: the default position (mid-reservoir)
    # sits inside the high-permeability sub-zone's own annotation below.
    draw_base_geometry(ax, "open interval\n(mass + advective heat exchange,\ninjection)",
                        reservoir_label_y=-1060)
    ax.annotate("Peaceman points:\nfluid enters the domain\nhere, one every 100m",
                xy=(0, OPEN_POINTS[3]), xytext=(-R_MAX * 0.42, OPEN_POINTS[3] + 120),
                fontsize=9, style="italic", ha="left", va="center",
                arrowprops=dict(arrowstyle="-", color="#222222", lw=0.8))

    # injection_zones.i's higher-permeability sub-zone - the whole point of this schematic
    # differing from the production one, so it gets the most visually distinct treatment: a
    # solid-filled band plus its own label, rather than just another annotation arrow.
    ax.add_patch(patches.Rectangle((0, HIGH_K_BOTTOM), R_MAX, HIGH_K_TOP - HIGH_K_BOTTOM,
                                    facecolor="#d62728", alpha=0.25, edgecolor="#d62728",
                                    linewidth=1.2, zorder=1))
    ax.annotate("high-permeability sub-zone\n(10x the surrounding reservoir,\ny=-1400 to -1600m) -\nreceives ~9x more flow\nthan its neighbors",
                xy=(R_MAX * 0.55, (HIGH_K_TOP + HIGH_K_BOTTOM) / 2),
                xytext=(R_MAX * 0.42, -1250),
                fontsize=8.5, style="italic", ha="left", va="center", color="#a81f1f",
                arrowprops=dict(arrowstyle="-", color="#d62728", lw=1.0))

    # Flow-direction arrow at the well itself, right at the open interval - the one thing that
    # visually distinguishes "this well injects" from the production schematic's own implied
    # upward extraction, without needing a second full legend.
    ax.annotate("", xy=(0, -1750), xytext=(0, -1550),
                arrowprops=dict(arrowstyle="->", color="#1f4fd6", lw=2.5),
                zorder=4)
    ax.text(35, -1650, "injection\nflow", fontsize=8, color="#1f4fd6", va="center")

    ax.set_title("Geometry on a radial (RZ) slice of the rate-controlled injection well")
    fig.tight_layout()
    fig.savefig("injection_schematic.png", dpi=150)
    print("Saved injection_schematic.png")


def main():
    make_production_schematic()
    make_injection_schematic()


if __name__ == "__main__":
    main()
