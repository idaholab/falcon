# HTM-coupled RTES

## Problem Statement

The circulation of hot fluid involved in RTES induces mechanical deformation due to pore pressure change and thermal expansion/shrinkage. In this example, we demonstrate the capability of FALCON for simulating hydro-thermo-mechanical (HTM) coupled problems. The Lower Tuscaloosa Sandstone formation with seasonal storage operation, similar to the [hydro-thermal coupling example](rtes_ht.md), is simulated in this example. The reader can refer [!cite](jin2022influence) for details of the problem.

## FALCON Input File 

### Materials

The coupling of mechanical deformation is coded in FALCON via porosity change, see [theory](https://mooseframework.inl.gov/falcon/modules/porous_flow/governing_equations.html) for details. A constitutive law is needed to related porosity with permeability for two-coupling. We find both the horizontal and vertical permeability of the Lower Tuscaloosa Sandstone layers can be fitted with porosity via the well-known Kozeny-Carmen relationship, using data from [!cite](doughty2013modeling). [fig:perm_poro] shows the fitted curve with Kozeny-Carmen exponent $m=2$ and $n=5$. 

!media media/permeability_porosity.png
  id=fig:perm_poro
  style=width:50%;
  caption= Fitted Kozeny-Carmen relationship between the measured porosity and measured permeability.

The other geomechanical related material parameters were extracted from [!cite](kim2014above), [!cite](doughty2013modeling) and listed in the following as:

!table
  id=tab:GeomechanicsParams
  caption=Geomechanical parameter values for HTM coupled problem with FALCON
| Parameter	|  Value |
|-----------|--------|
| Rock shear modulus | 8.38 GPa |
| Poisson's ratio | 0.225 |
| In-situ vertical stress | 67.3 MPa |
| In-situ horizontal stress | 42.9 MPa |
| Thermal expansion coefficient | $1.0^{-5}$ |

The updated material section of the input file is then as

!listing examples/rtes_htm/Cranfield_HTM_continous_season_caps_water_newboundary_coarse.i block=Materials

Note for each layer of the simulated reservoir, we calculated the pemeability scale value of $\kappa_0$ as the reference porosity varies layer by layer for the [Kozeny Carman](https://mooseframework.inl.gov/falcon/source/materials/PorousFlowPermeabilityKozenyCarman.html) relationship. The two wells were treated as two high permeabeable columns with injection and extraction only at the top of the two wells, as explained in the [hydro-thermal coupling case](rtes_ht.md).

### Geometry and boundary conditions

The geometry and boundary conditions for heat transfer and fluid flow is the same as the [HT coupling case](rtes_ht.md). For mechanical boundaries, we applied a fixed displacement in the vertical direction at the bottom surface (z=-24 m) and a fixed horizontal displacement for the two side surfaces (y=-10 m, x=-10 m). To ensure effective stress equilibrium, we applied normal stress boundary conditions of $\sigma_h$ = 42.9 MPa for the two side surfaces (y=10 m, x=10 m) and $\sigma_v$ = 67.3 MPa for the top surface (z=24 m). The input section for boundary condition is

!listing examples/rtes_htm/Cranfield_HTM_continous_season_caps_water_newboundary_coarse.i block=BCs

Instead of constant temperature injection at the injection well, a sinusoid temperature evolution with a peak temperature of 180 $^oC$ in summer and a valley temperature of 126 $^oC$ in winter was used to mimic the variation of hot water from surface solar energy. This was achieved through input file function as

!listing examples/rtes_htm/Cranfield_HTM_continous_season_caps_water_newboundary_coarse.i block=Functions

### Operation 

For the THM simulations, two modes of operation were considered. In the flow-through operation, one well is always the injection well and the other well is always the production well, and fluid always flows through the formation in the same direction from the injection well to the production well. In the push-pull operation, one well acts as the injection/production well and the other well acts as the supply well. During summer, hot water is injected, and during winter, hot water is produced from the same well, thus the flow direction in the reservoir is reversed. For both the operational modes, the supply well always has the same magnitude of flow rate as the injection/production (hot/cold) well, to minimize pressure changes in the reservoir.

The two operation modes were realized by the combination of Functions, Controls and DiracKernals. Take push-pull operation case as an example, its input sections of Control and DiracKernals are

!listing examples/rtes_htm/Cranfield_HTM_push-pull_season_caps_water_newboundary_coarse.i block=Functions
!listing examples/rtes_htm/Cranfield_HTM_push-pull_season_caps_water_newboundary_coarse.i block=Controls

## Results

Both the push-pull and flow-through operation cases were simulated for 10 years. [fig:results_con] and [fig:results_pp] show the predicted pore pressure and temperature evolution at the two wells over 10 years of flow-through and push-pull operation, respectively. 

!media media/benchmark_moose_tough_continous_season_10.png
  id=fig:results_con
  style=width:100%;
  caption= Simulated pore pressure and temperature evolution at the injection and production wells for the continous operation scenario.

!media media/benchmark_moose_tough_pull-push_season_10.png
  id=fig:results_pp
  style=width:100%;
  caption= Simulated pore pressure and temperature evolution at the hot and cold wells for the push-pull operation scenario.

!bibtex bibliography