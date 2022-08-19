!alert construction title=Develop Documentation prefix=False
All pages on this website are currently under development and do not yet serve as general documentation of the [FALCON repository](https://github.com/idaholab/falcon).

# HT-coupled RTES Benchmark between FALCON and TOUGH

Wencheng Jin
## Problem Statement
Reservoir Thermal Energy Storage (RTES), also called Geological Thermal Energy Storage (GeoTES) or Aquifer Thermal Energy Storage (ATES), is the concept of injecting hot fluid into the subsurface formation for energy storage and extracting it for later use. This example demonstartes the MOOSE-based FALCON can obtain the same quantitative results as compared to the [TOUGH3](https://tough.lbl.gov/software/new-release-tough3/) code for the heat transfer and fluid flow coupled problems. 

!media media/TOUGH_benchmark.png
  id=fig:benchmark
  style=width:90%;
  caption= Model set up comparison between TOUGH and FALCON for the Lower Tuscaloosa formation.

The Lower Tuscaloosa formation located at the Cranfield site was chosen as the storage site. [fig:benchmark] shows the geometry and boundary conditions for the two models built for the TOUGH and FALCON solvers, respectively. The formation thickness is about 24 m and is located at a depth of 3000 m.  Given the symmetry of the 5-spot well configuration, a 1/8 TOUGH model and a quarter FALCON model were built. Note the TOUGH is a finite difference method based solver while FALCON is a finite element method based solver, the quarter FALCON model has a better convergence rate. The cap-/bed-rock layers of the TOUH model were replaced by undrained flow boundary conditions and an analytical solution for heat conduction. While cap-/bed-rocks were explicitly simulated in the FALCON model with both no-flux of heat and fluid applied to the top and bottom surfaces. All the side surfaces for both the FALCON and TOUGH models are symmetrical boundaries. 

[fig:benchmark] also shows the permeability profile logged from one well, and the permeability with layers were used for both models. 

!listing examples/RTES_benchmark/Cranfield_HT_season_caps_water_newboundary_coarse.i block=Materials

