# HT-coupled RTES Benchmark between FALCON and TOUGH

Wencheng Jin
## Problem Statement
Reservoir Thermal Energy Storage (RTES), also called Geological Thermal Energy Storage (GeoTES) or Aquifer Thermal Energy Storage (ATES), is the concept of injecting hot fluid into the subsurface formation for energy storage and extracting it for later use. This example demonstartes the MOOSE-based FALCON can obtain the same quantitative results as compared to the [TOUGH3](https://tough.lbl.gov/software/new-release-tough3/) code for the heat transfer and fluid flow coupled problems. 

!media media/TOUGH_benchmark.png
  id=fig:benchmark
  style=width:100%;
  caption= Model set up comparison between TOUGH and FALCON for the Lower Tuscaloosa formation.

The Lower Tuscaloosa formation located at the Cranfield site was chosen as the storage site. [fig:benchmark] shows the geometry and boundary conditions for the two models built for the TOUGH and FALCON solvers, respectively. The formation thickness is about 24 m and is located at a depth of 3000 m.  Given the symmetry of the 5-spot well configuration, a 1/8 TOUGH model and a quarter FALCON model were built. Note the TOUGH is a finite difference method (FDM) based solver while FALCON is a finite element method based solver, the quarter FALCON model has a better convergence rate. The cap-/bed-rock layers of the TOUH model were replaced by undrained flow boundary conditions and an analytical solution for heat conduction. While cap-/bed-rocks were explicitly simulated in the FALCON model with both no-flux of heat and fluid applied to the top and bottom surfaces. All the side surfaces for both the FALCON and TOUGH models are symmetrical boundaries. 

A typical RTES operation of injection-storage-extraction-rest seasonal cycles were simulated. During the injection season, we inject hot fluid with a constant injection temperature of 180 degree  into the injection well and extract fluid from the production well at the same fixed rate of 3 kg/s. During the extraction season, we inject cold fluid with the same temperature as the formation initial temperature T0 = 126 degree C into the injection well and extract fluid from the production well at the same fixed rate of 3 kg/s. No fluid injection or extraction is conducted during the storage and rest seasons. For this problem, we detail the the input file of FALCON int the following section, and the input file for TOUGH3 can be downloaded [here](examples/rtes_benchmark/tough.inp). 

## FALCON Input File & Results 
[fig:benchmark] also shows the permeability profile logged from one well of the Lower Tuscaloosa Sandstone formation. The horizontal and vertical porosity and permeability data were obtained from [!cite](doughty2013modeling) and averaged over the zone of interest and discretized into 20 layers. For the first FALCON simulation, the material section of the input file is

!listing examples/rtes_benchmark/Cranfield_HT_season_caps_water_oldboundary_noComp_coarse.i block=Materials

The green dash-dotted lines in [fig:comparison] show the predicted pore pressure and temperature evolution of 1 year cycle at the injection and extraction wells. As compared to the TOUGH prediction marked as black lines, the predicted pore pressure of FALCON simulation significantly exceeds the TOUGH prediction. A careful comparison between the two input files, we found that the FALCON simulatuion used a constant porosity and permeability, while the TOUGH simulation used a pore compressibility of 3 $$GPa^-1$$. The pore compressibility implentation in FALCON is different from TOUGH, the reader can refer the [porosity](https://mooseframework.inl.gov/modules/porous_flow/porosity.html) theory for details. The updated material section is listed in the following, where a Biot coefficient of 0.8 and a bulk modulus of 150 MPa were used:

!listing examples/rtes_benchmark/Cranfield_HT_season_caps_water_oldboundary_coarse.i block=Materials

The blue dotted lines show the FALCON prediction with updated input file. The pore pressure at both the injection and extraction wells match each other perfectly. However, the yielded temperature prediction at the extraction well still has significant discrepancy during the storage season as shown in [fig:comparison]. With a careful examination of the two input files, we found this is due to the boundary condition setup of the two wells. The TOUGH solver is based on FDM, and the two wells were realized by two columns of elements with extremly high permeability. While, line sinks were used for FALCON models. Following the same boundary treatment as the TOUGH model, two columns of elements were selcted as different blocks as compared to the formation. The material input for these two blocks are 

!listing examples/rtes_benchmark/Cranfield_HT_season_caps_water_newboundary_coarse.i block=Materials

The injection and extraction points were only located at the top of those two columns, they were realized by using [DiracKernels](https://mooseframework.inl.gov/modules/porous_flow/sinks.html). See the following input file section for detail:

!listing examples/rtes_benchmark/Cranfield_HT_season_caps_water_newboundary_coarse.i block=DiracKernels

Note the above Dirac Kernels were activated partially to realize the injection-storage-extraction-rest operation cycle, a control block in the input file was used to realize that as

!listing examples/rtes_benchmark/Cranfield_HT_season_caps_water_newboundary_coarse.i block=Controls

With these modification, the FALCON predicted results, shown as red dashed lines in in [fig:comparison], match well with the TOUGH prediction (black solid lines). We conclude that FALCON is a robust solver to simulate thermo-hydraulic coupled problems. The readers can refer [!cite](jin2022influence) for detialed discussion of the benchmark study.

!media media/benchmark_moose_tough_constant_season_1.png
  id=fig:comparison
  style=width:100%;
  caption= Comparison of pressure and temperature evolutions at the injection and production wells with a year of injection-storage-extraction-rest cycle with different boundary conditions and model set-ups. For the benchmark study, we used a constant injection temperature at 180 degree C, and the injection starts at the beginning of the simulation.

##
The author would like to acknowledge Christine Doughty at the Lawrence Berkeley National Laboratory for performing TOUGH simulations and providing the corresponding input file.

!bibtex bibliography



