# RTES Stochastic Simulation: Parametric Sweeping 

## Problem Statement

The performance of RTES is highly relevant to the formation characteristics of the site and operational conditions, which are the critical attributes of the techno-economic performance of a potential site. This example demonstrates the capability of FALCON to generate synthetic data for RTES potential site identification using automated stochastic simulation. Following [!cite](jin2022machine) and [!cite](jin2021geothermal), a generic caprock-reservoir-bedrock formation with varying formation characteristics (permeability, porosity, thermal conductivity, thickness, in-situ temperature and pore pressure) was simulated. [fig:geometry] shows the boundary conditions and the geometry of the generic formation. Note a common doublet well configuration was adopted for this case, and half of the domain was simulated due to symmetry. In addition to the formation characteristics, the operational conditions of injection rate, fluid injection temperature, and well distance were also investigated to reveal their influence on RTES performance. 

!media media/rtes_stochastic_geometry.png
  id=fig:geometry
  style=width:100%;
  caption= Geometry and boundary conditions of the generic formation with doublet system for simulation.

## FALCON input files

### Physics simulation input file

In this example, only the physics of heat transfer and fluid flow is considered, and the formation is assumed homogeneous and fully saturated with uniform in-situ temperature and pore pressure. To enable the flexbility of convergence implemented in unsaturated kernels, we used the [PorousFlowUnsaturated Action](https://mooseframework.inl.gov/source/actions/PorousFlowUnsaturated.html) as

```
[PorousFlowUnsaturated]
  relative_permeability_type = COREY
  relative_permeability_exponent = 0
  add_darcy_aux = true
  coupling_type = ThermoHydro
  gravity = '0 0 -9.8'
  porepressure = porepressure
  temperature = temperature
  fp = tabulated_water
  use_displaced_mesh = false
[]
```

*Mesh:* The generic formation was meshed using [CUBIT](https://cubit.sandia.gov/). As a result, the thickness of formation was descretized into 10 cases with thickness ranging from 10 meters to 100 meters. The loading of the Exodus mesh file in input file is via

```
[Mesh]
  [./fmg]
    type = FileMeshGenerator
    file = base_final_50.e
  []
[]
```

*Initial Condition:* Given the initial temperature and pore pressure is correlated with depth in general as

\begin{equation}
P_0(Z)=
\begin{cases}
\rho_f gZ, \quad Z<2000 \\
\rho_f gZ + (Z-2000)\rho_f g, \quad  Z\geq 2000
\end{cases}
\label{eq:P0}
\end{equation}
\begin{equation}
T_0(Z) = 26.67 + 0.02005Z
\label{eq:T0}
\end{equation}

We coded the equation in the input file as

```
depth =  2000 
pp_ini_bc = ${fparse if(depth<=2000, 1000*9.8*depth, 1000*9.8*depth+(depth-2000)*9.8*1000)}
T_ini_bc  = ${fparse (80+3.28084*depth*1.1/100-32)*5/9 + 273.15}
```

and we assigned the initial condition as

!listing examples/rtes_stochastic/base_cycle_50.i block=ICs

*Boundary Condition:* The simulated domain has a 1 km length in the horizontal direction, and we assigned drained boundary conditions with pore pressure and temperature equal to the initial values for all side faces, except the symmetrical surface. While the top and bottom surfaces were undrained. The input file section is
!listing examples/rtes_stochastic/base_cycle_50.i block=BCs

*Operation:* In order to perform parametric study of operational conditions, we first defined variables in input file as

```
distance_between_wells = 200
pro_well_x = ${fparse 0.1 + distance_between_wells / 2}
inj_well_x  = ${fparse 0.1 - distance_between_wells / 2}
well_length = 50  #fixed number in accordance with the mesh
aquifer_mid= ${fparse 0 + 0}
aquifer_top= ${fparse 0 + well_length/2}
aquifer_bot= ${fparse 0 - well_length/2}
#---Define a scalar variable to sweep 
inj_ext_exponent = 1.5
inj_ext_flux= ${fparse 10^inj_ext_exponent / well_length / 2 }
injection_temp = 473.15
```

The above variables help to assign injection parameters for the operation. A typical seasonal operation with injection-storage-extraction-rest cycle was simulated. In the summer, cold fluid is extracted from the cold well and heated to an assigned temperature, then re-injected into the reservoir through the hot well at the same rate as the extraction. The same injection and extraction rates at the charging summer period ensure the pore pressure disturbance in the reservoir is minimized. In the winter, fluid is extracted from the hot well for the generation of electricity with the post-generation energy-depleted fluid  being injected back into the reservoir through the cold well. The extraction and injection rates during the winter discharging period  also remain the same as the summer charging time, and the injected fluid temperature into the cold well is the same as the initial reservoir temperature. For the fall and spring seasons, no fluid circulation is performed. Note in this example, high-temperature RTES is targeted to generate electricity when thermal energy is extracted. The energy extraction period is terminated when the temperature of the extracted hot well fluid is reduced by 
20\% in relation to the difference between the fluid
injection temperature and the initial reservoir
temperature, i.e., we stop discharging when the temperature of the hot well $T_{hot}$ satisfies $(T_{hot}-T_0)/(T_i-T_0)<80\%$. This high cut-off temperature is adopted to ensure the extracted hot fluid can be used for electricity generation. This operation was achieved by the combination of [DiracKernels](https://mooseframework.inl.gov/modules/porous_flow/sinks.html), [Controls](https://mooseframework.inl.gov/syntax/Controls/index.html), [Functions](https://mooseframework.inl.gov/syntax/Functions/index.html), and [Postprocessors](https://mooseframework.inl.gov/syntax/Postprocessors/index.html) of FALCON. 

First, we defined three groups of DiracKernels to realize the charging, discharging and rest operations from the two wells. Note the predefined variables of injection/extraction rate, well locations, and injection temperature are used as

!listing examples/rtes_stochastic/base_cycle_50.i block=DiracKernels

The above three groups of DiracKernels, which control the well operation, are sequentially activated using Controls. 

!listing examples/rtes_stochastic/base_cycle_50.i block=Controls

These controls are based on Functions, which are constructed in terms of season only for the charging operation and in terms of season and postprocessor values for the charging and rest operationsn as

!listing examples/rtes_stochastic/base_cycle_50.i block=Functions

The b_swith value is from the following termination postprocessor, its value changes to 1 when  $(T_{hot}-T_0)/(T_i-T_0)<80\%$ and changes to 0 when $(T_{hot}-T_0)/(T_i-T_0)\geq80\%$:

```
  [./termination]
    type = PorousFlowTemperatureDropTerminator
    enthalpypostprocessor = hotwell_enthalpy_J
    masspostprocessor = hotwell_mass_kg
    T_inj = ${injection_temp}
    T_init = ${T_ini_bc}
    P_drop = 80
    timepostprocessor = time 
  [../]
```

*Materials:*  Constant material parameters,including porosity, permeability, thermal conductivity, specific heat and density, are assigned with valuse listed in [tab:stochastic]. The input file section for material is

!listing examples/rtes_stochastic/base_cycle_50.i block= Materials

Note the variables of reservoir permeability and thermal conductivity are defined at the start of the input file are used to in this section.

```
#---Define a scalar variable to replace the tensor components
Tcond_aquifer = 2.0
#---Define a scalar variable to replace the tensor components
perm_exponent = -12
perm_aquifer = ${fparse 10^perm_exponent}
```

!table
  id=tab:stochastic
  caption=Modeling parameters with uncertain/fixed values used for the simulations
| Parameters | Symbol | Units | Reservoir | Caprock|
|-----------|--------|--------|--------|--------|
| Permeability | $\kappa$ | $m^2$ | $10^{-15}-10^{-11}$    | $10^{-18}$  |
| Porosity |   $\phi$    |   -    |    $0.01-0.30$   |  $0.01$ |
| Themal Conductivity |  $\lambda$     |  $W/(m\cdot{K})$     |   $2.0-4.0$    |  $2.5$|
| Specific Heat |  $C_m$     |   $J/(kg\cdot{K})$   |  930      | 1000 |
| Grain Density |   $\rho_m$    |    $kg/m^3$   |  2650     | 2500 |
| Formation Thickness |   $H$    |   $m$    |   10-100    | 50 |
| Well Distance |  $D$     |  $m$     |   100-200    | - |
| Flow Rate |   $Q$    |  $kg/s$     |  1-1000     | - |
| Inj. Temperature |  $T_i$      |  $^oC$     |  100-300     | - |
| Formation Depth |   $Z$    |    $m$   |   1000-3000    | - |


### Stochastic sampling input file

The [stochastic Tools Module](https://mooseframework.inl.gov/modules/stochastic_tools/index.html) implemented in MOOSE was used to perform automated parameteric sweeping. As shown in [tab:stochastic], uniform distributions with min-max range were used for all sweeping parameters except the ones with several orders of magnitude difference in min-max values (i.e., permeability, flow rate). A logorithmic scaling was performed for the two parameters and their exponents of min-max values was used to define the uniform distribution ranges. Accordinlgy, the distribution section is

!listing examples/rtes_stochastic/master_cycle_50.i block=Distributions

A Monte Carlo sampler was adopted to sample the values of depth, injection fluid temperature, flow rate exponent, distance between wells, reservoir permeability exponent, porosity, thermal conductivity from their corresponding uniform distribution defined previously. The input file seciton is

!listing examples/rtes_stochastic/master_cycle_50.i block=Samplers

In order to assign the sampled values to the corresponding parameters of the physics input file, a Control section is needed to connect the exact variables inside the physics simulation input file to the distributions of stochastic simulation input file as

!listing examples/rtes_stochastic/master_cycle_50.i block=Controls

Note the sequence of param_names in Control section needs to match with the distributions in montecarlo section. The use of defined variables in the physics simulation input file greatly simplfies the connection between the two input files.

The transfer section was used to collect RTES performance metrics of each realized sample. It includes recovery efficiencty (the ratio of extracted thermal energy over the injected energy), storage capacity (total amount of energy injected each year) and operating time (the function time of extraction to generate electricity). These performance metrics were defined in Postprocessors of the physics simultion input file,

```
  [./recovery_rate]
    type = PorousFlowRecoveryRateSeason
    hotwellenergy = hotwell_enthalpy_J
    coldwellenergy = coldwell_enthalpy_J
    InjectionIndicator = is_summer
    ProductionIndicator = is_winter
  [../]
  [./total_energy]
    type = PorousFlowEnergyAccumulator
    hotwellenergy = hotwell_enthalpy_J
    coldwellenergy = coldwell_enthalpy_J
    ProductionIndicator = is_winter
  [../]
  [./total_recovery_time]
    type = PorousFlowRecoveryTimeAccumulator
    targetpostprocessor = is_winter
    dtpostprocessor = step_dt
  [../]
```
and supplied in the stochatic simulation input file as

```
[Transfers]
  [results]
    type = SamplerPostprocessorTransfer
    multi_app = runner
    sampler = ${sampler}
    to_vector_postprocessor = results
    from_postprocessor = 'recovery_rate total_recovery_time total_energy'
  []
[]
```

The final section of the stochastic simulatiom input file is the MultiApps section to define the physics simultion input file that it can work with as

```
[MultiApps]
  [runner]
    type = SamplerFullSolveMultiApp
    sampler = ${sampler}
    input_files = 'base_cycle_50.i'
    mode = batch-reset
    ignore_solve_not_converge = true
  []
[]
```

## Results

As shown in [fig:demoseaso], we simulate 10 years for the seasonal cycle scenario of each case. The pore pressure evolution sampled from a node close to the middle of the hot well shows spikes at the start of injection of each summer, which is resulted from the line sink boundary condition and fluid viscosity dropping with increasing temperature. This effect is less significant over time because the RTES system has been warmed up as a portion of thermal energy is not extracted during the winter in each cycle. The warming trend is clearly shown in [fig:demoseaso] as the temperature at the end of each cycle continues to increase throughout the 10 years of operation.

!media media/demo_season.png
  id=fig:demoseaso
  style=width:100%;
  caption= Pore pressure and temperature evolution at the middle of the hot well for the scenario of injection-storage-extraction-rest seasonal cycle of 10 years operation. Note that the extraction operation only occurs when the temperature of the fluid in the hot well ($T_{hot}$) exceeds the cut-off temperature (i.e.,$(T_{hot}-T_0)/(T_i-T_0)\geq 80\%$), and the hot well temperature can raise again due to heat conduction when extraction stops. This temperature raise will restart the extraction which results in a zigzag-shaped curve during the extraction season as shown above. Note the temperature of the extracted fluid from the hot well has a discrepancy with the temperature at the middle of the well.

[fig:input] shows the histograms of the formation parameters (green) and the operating conditions (magenta) of all the realized simulations. The screening-out process of the physically meaningless cases and the unreasonable cases in practice also results in the non-uniform distribution in flow rate and permeability.

!media media/season_input_hist.png
  id=fig:input
  style=width:60%;
  caption= Histograms of the input parameters for all the realized simulations of the seasonal cycle operation.  The y-axis stands for the realized simulation numbers after screening. We color-code the formation characteristics in green and the operation conditions in magenta.

[fig:output] plots the histograms of the RTES performance metrics for the seasonal cycle case. Because we stop and restart the extraction process during the winter season according to the fluid temperature in the hot well, the recovery efficiency is high, and the operation time spans from 50 days to 80 days with a close to uniform distribution. The predicted annual storage capacity is close to a power-law distribution, and we plotted it in the logarithmic scale. 

!media media/season_output_hist.png
  id=fig:output
  style=width:30%;
  caption= Histograms of the output parameters for all the realized simulations of the seasonal cycle operation.Note the performance metrics are all annual average values.

  <!-- !bibtex bibliography -->