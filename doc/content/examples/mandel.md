# Mandel's consolidation

A sample's dimensions are $-a \leq x \leq a$ and $-b \leq y \leq b$,
and it is in plane strain (no $z$ displacement).  It is squashed with
constant normal force by impermeable, frictionless plattens on its top
and bottom surfaces (at $y = \pm b$).  Fluid is allowed to leak out
from its sides (at $x = \pm a$), but all other surfaces are
impermeable.  This is called Mandel's problem and it is shown
graphically in []

The interesting feature of this problem (apart from that it can be
solved analytically) is that the porepressure in the sample's center
increases for a short time after application of the force.
This is because the leakage of the fluid from the sample's sides
causes an apparent softening of the material near those sides.  This
means stress concentrates towards the sample's center which causes an
increase in porepressure.  Of course, eventually the fluid totally
drains from the sample, and the porepressure is zero.  As the fluid
drains from the sample's sides the plattens move slowly towards each
other.


As is common in the literature, this is simulated by considering the
quarter-sample, $0\leq x \leq a$ and $0\leq y\leq b$, with
impermeable, roller BCs at $x=0$ and $y=0$ and $y=b$.  Porepressure is
fixed at zero on $x=a$.  Porepressure and displacement are initialised
to zero.  Then the top ($y=b$) is moved downwards with prescribed
velocity, so that the total force that is induced by this downwards
velocity is fixed.  The velocity is worked out by solving Mandel's
problem analytically, and the total force is monitored in the
simulation to check that it indeed remains constant.

This example solves the problem using a fixed-stress split: a mechanics
"master" app and a PorousFlow "sub" app are coupled through MOOSE's MultiApp
system, with the master app driving a fixed-point (Picard) iteration between
the two physics each timestep via a `DefaultMultiAppFixedPointConvergence`.
This is the MultiApp counterpart to the monolithic (fully-coupled)
`mandel_constM` test in the PorousFlow module's own test suite, which solves
the identical mesh, materials and BCs in a single app. The fixed-point
iteration here is converged to a tight relative tolerance (not capped at a
fixed iteration count), so the split solution reproduces the monolithic
result to within about 1e-6 relative.

!listing examples/fixedStressMultiApp/mandel_master.i

!media media/mandel_results.png
  id=fig:mandelresults
  style=width:100%;
  caption= The horiztonal and vertical displacement and the pore pressure at different locations of the platten as a function of time.
