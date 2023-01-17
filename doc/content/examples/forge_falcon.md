# Utah FORGE Native State Model

## Problem Statement

The Frontier Observatory for Research in Geothermal Energy (FORGE) site is a multi-year initiative funded by the U.S. Department of Energy for enhanced geothermal system research and development. This example presents the development of the native state simulation (i.e., pore pressure, stress, and temperature) of the FORGE site using material property values obtained from the site characterization activities. A detailed description can be found in [!cite](liu2022forge).

!media media/forge_geometry.png
  id=fig:forgemesh
  style=width:70%;
  caption= Geometry and finite element mesh of the FORGE site model.

The FORGE site is located inside the southeast margin of the Great Basin near the town of Milford, Utah. The site stratigraphy is divided into two units composed of crystalline plutonic rock and overlying bedded alluvium and volcanic deposits. A finite element mesh was customized to accurately capture the two units with a surface to separate them as shown in [fig:forgemesh]. In FALCON input file, this mesh is imported via

```
[Mesh]
  file = forge_model80x40x40.msh
  use_displaced_mesh = false
[]
```

This `*.msh` input file can be downloaded from the [GDR website](https://gdr.openei.org/submissions/1397).

## Input File

### Governing Equations

The native state model considers geomechanics, fluid flow, and heat transfer. Instead of using `Actions`, such as the `PorousFlowUnsaturated` `Actions` in the previous RTES examples, we now build the THM coupled governing equation using `Kernals` from the scratch. First, we define the primary unknows, which are the pore pressure, temperature, and displacement as

```
[Variables]
  [./pressure]
  [../]
  [./temperature]
    scaling = 1E-8
  [../]
  [./disp_i]
    scaling = 1E-10
  [../]
  [./disp_j]
    scaling = 1E-10
  [../]
  [./disp_k]
    scaling = 1E-10
  [../]
[]
```

Note the scaling factors are used to achieve a better convergence rate, see the [instruction](https://mooseframework.inl.gov/modules/porous_flow/convergence.html) for details. 

The complete governing equations of the THM coupled problem are detialed in the [theory page](https://mooseframework.inl.gov/modules/porous_flow/governing_equations.html). For this FORGE simulate case, we simplify them as
\begin{equation}
	\nabla \cdot (\bm{\sigma}-\alpha p \bm{I} - \beta T\bm{I}) + \rho \bm{g}=0\\
	\alpha \nabla \cdot \dot{\bm u} + \frac{1}{M}\dot p - \nabla \cdot(\frac{\bm \kappa}{\mu} \nabla p)= q \\
	\frac{1}{\gamma}\dot{T} - \nabla \cdot (\bm\lambda\nabla T - \sum_jh_jF_j) = h_s
\end{equation}
where $p$ and $T$ denote pore presure and temperature, while $\bm{\sigma}$ and $\bm{I}$ are the stress and identity tensors. $\alpha$ is the Biot’s constant, and $\beta$ is the thermal expansion coefficient for rocks, $\rho$ represents the density of porous media including both rock skeleton and filled fluids, and $\bm g$ the gravity vector. For the mass convervatiom equation, where $\bm u$ is the displacement vector of the solid phase, ${\bm \kappa}$ is the permeability tensor of porous media, $q$ is the fluid flow source, and $M$ is the Biot’s modulus. For the energy conservation equation,  $\bm\lambda$ is the thermal conductivity tensor for whole rock media, $h_j$ the specific enthalpy for phase j, $F_j$ is the advective Darcy flux, and $h_s$ is the heat source.

Note the objective of this simulation is to obtain the native state of the FORGE site with a steady state simulation. The pressure ($\dot p$) and temperature ($\dot T$) time derivative terms, in above equations are omitted. Given the extreme low permeability, the heat conduction term due to fluid flow is also omitted. We also assume the fluid in FORGE site is in the liquid state under the native state. With above reasoning, the `Kernals` section for the governing equation is listed in the following:

```
[Kernels]
  [./Darcy_flow]
    type = PorousFlowAdvectiveFlux
    fluid_component = 0
    variable = pressure
    gravity = '0 0 -9.8'
  [../]
  [./heat_conduction]
    type = PorousFlowHeatConduction
    variable = temperature
  [../]
  [./grad_stress_i]
    type = StressDivergenceTensors
    variable = disp_i
    component = 0
  [../]
  [./grad_stress_j]
    type = StressDivergenceTensors
    variable = disp_j
    component = 1
  [../]
  [./grad_stress_k]
    type = StressDivergenceTensors
    variable = disp_k
    component = 2
   # displacements = 'disp_i disp_j disp_k'
  [../]
  [./poro_i]
    type = PoroMechanicsCoupling
    variable = disp_i
    porepressure = pressure
    component = 0
  [../]
  [./poro_j]
    type = PoroMechanicsCoupling
    variable = disp_j
    porepressure = pressure
    component = 1
  [../]
  [./poro_k]
    type = PoroMechanicsCoupling
    variable = disp_k
    porepressure = pressure
    component = 2
  [../]
  [./weight]
    type = Gravity
    variable = disp_k
    value = -9.8
  [../]
[]
```

In order to obtain the secondary variables, such as the stress tensor, fluid density, viscosity, etc, the `AuxKernels` section needs to explicitly request these output.


### Initial and boundary conditions

##### For fluid flow field, as shown in [fig:forgebc], we applied

- 101 $kPa$ atmospheric pore pressure on the top surface,
- 34 $MPa$ pore pressure on the bottom surfaced,
- and Zero flux for all side surfaces

!media media/forge_bc.png
  id=fig:forgebc
  style=width:100%;
  caption= Illustration of boundary conditions for coupled three field.

The input lines are

```
[./pore_pressure_top]
    type = DirichletBC
    variable = pressure
    boundary = 'surface_40m'
    value = 101325 #atmospheric pressure in Pa-assumes water level at ground surface
  [../]
  [./pore_pressure_bottom]
    type = DirichletBC
    variable = pressure
    boundary = 'bottom_40m'
    value = 34000000
[../]
```

##### For heat transfer, we applied

-  26$^\circ C$ atmospheric temperature on the top surface,
-  a distributed temperature on the bottom surface using a function 
-  and zero heat flux boundary conditions for all side surfaces.

```
[./Side_Z_Minus_T]
    type = FunctionDirichletBC
    variable = temperature
    boundary = 'bottom_40m'
    function = T_on_face_Z_Minus
  [../]
  [./Side_Z_Plus_T]
    type = DirichletBC
    variable = temperature
    boundary = 'surface_40m'
    value= 299
  [../]
[Functions] # temperature on boundary surfaces
  [./T_on_face_Z_Minus]
    type = PiecewiseBilinear0
    data_file = bottom_2021_11_10_plus_50.csv
    xaxis = 1
    yaxis = 0
  [../]
[]
```

Note the temperature unit is `kelvin`, and the temperature distribution function is built on the 'PiecewiseBilinear0' kernal using the `x` and `y` coordinates. The bottom_2021_11_10_plus_50.csv file contains the distributed temperature.

##### For the solid deformation, we applied

- fixed displacement of the bottom and two side surfaces for the components perpendicular to each surface,
- a uniform 101 $kPa$ atmospheric pressure at the top surface,
- and normal tractions to the two side surface using the `z` coordinate independent functions.

```
    [./roller_sigma_h_min]
    type = DirichletBC
    preset = true
    variable = disp_i
    value = 0
    boundary = 'bottom_40m_granitoid_40m_front granitoid_40m_surface_40m_front'
  [../]
  [./roller_sigma_h_max]
    type = DirichletBC
    preset = true
    variable = disp_j
    value = 0
    boundary = 'bottom_40m_granitoid_40m_right granitoid_40m_surface_40m_right'
  [../]
  [./bottom_z]
    type = DirichletBC
    preset = true
    variable = disp_k
    value = 0
    boundary = 'bottom_40m'
  [../]
    [./S_h_min_normal]
    type = FunctionNeumannBC
    variable = disp_i
    boundary = 'bottom_40m_granitoid_40m_back granitoid_40m_surface_40m_back'
    function  = '-15709*(1785-z)'
  [../]
  [./Side_Y_Plus_normal]
    type = FunctionNeumannBC
    variable = disp_j
    boundary = 'bottom_40m_granitoid_40m_left granitoid_40m_surface_40m_left'
    function  = '-19640*(1785-z)'
  [../]
  [./Side_Z_Plus_traction_Z]
    type = NeumannBC
    variable = disp_k
    boundary = 'surface_40m'
    value = -101325
  [../]
```

The negative sign of the atmospheric pressure indicates the opposite direction of pressure from traction.

#### Initial condition

We only initialized the pore pressure and temperature approximately using the same `z` depenedent functions as

```
[ICs]
  [temperature]
    type = FunctionIC
    variable = temperature
    function = '616 - ((2360+z)*7.615e-2)'
  []
  [pressure]
    type = FunctionIC
    variable = pressure
    function = '3.5317e7 - ((2360+z)*8455)'
  []
[]
```

### Material parameters and constitutive models

[tab:materialParams] lists the values of the material parameters, in which we assume isotropic and homogeneous material distribution for each of the two layers. Accordingly, all the material values are constants, except a relative permeability was used for better convergence. The standard Equation of State for water & steam in IAPWS-IF97 is chosen for the fluid, and we assume the simulation domain is fully saturated with a single fluid phase. For mechanical deformation, a constant thermal expansion coefficient was used for both layers with a reference temperature of 20$^\circ$C, and the small strain linear elastic constitutive law was used.

!table
  id=tab:materialParams
  caption=Material parameter values for the FORGE native state simulation
| Parameter	|  Unit | Sedimentary Layer Value | Granitoid Layer Value |
|-----------|--------|--------|--------|
| Permeability | $m^2$ | 1E-14 | 1E-18 |
| Porosity | - | 0.12 | 0.001|
| Grain density | kg/$m^3$ | 2500 | 2750 |
| Specific heat  | $J/kg\cdot K$ | 830 | 790 |
| Thermal conductivity | $W/m\cdot K$ | 2.8 | 3.05 |
| Young's Modulus | GPa | 30 | 65 |
| Poisson's ratio | -  | 0.3 | 0.3 |
| Biot Coefficient | - | 0.47 | 0.47 |
| Thermal expansion Coefficient | - | 6E-6| 6E-6 |

The complete material input section is

!listing examples/forge/forge_native.i block=Materials

### Postprocessor and Excution

In order to validate the simulation, we extract the temperature, pore pressure and stress tensor distribution along the survey wells in FORGE, which include wells 16A, 58-32, 78-32, 78B-32, and 56-32. This was achived via the  `PointValueSampler` [sampler](https://mooseframework.inl.gov/source/vectorpostprocessors/PointValueSampler.html) via the well trajectory coordinates.

A steady state solver was requested in the `Executioner` section as

!listing examples/forge/forge_native.i block=Executioner

## Results

[fig:forgeresults] presents the simulated results along the trajectory of Well 56-32, a comparison between the in situ measurement and the prediction indicate the accuracy of the simulation.

!media media/forge_well_56_32_demo.png
  id=fig:forgeresults
  style=width:100%;
  caption= Pore pressure, temperature, and stress distribution along the trajectory of the Well 56-32.
