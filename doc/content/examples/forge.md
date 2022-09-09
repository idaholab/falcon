# Utah FORGE Native State Model

## Problem Statement
The Frontier Observatory for Research in Geothermal Energy (FORGE) site is a multi-year initiative funded by the U.S. Department of Energy for enhanced geothermal system research and development. This example demonstartes that the MOOSE-based FALCON can simulate the native state distribution (i.e., pore pressure, stress, and temperature) of the FORGE site with various in-situ data constraints. A detailed description can be found in [!cite](liu2022forge).

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
$$
\begin{align}
	\nabla \cdot (\bm{\sigma}-\alpha p \bm{I} - \beta T\bm{I}) + \rho \bm{g}=0\\
	\alpha \nabla \cdot \dot{\bm u} + \frac{1}{M}\dot p - \nabla \cdot(\frac{\bm \kappa}{\mu} \nabla p)= q \\
	\frac{1}{\gamma}\dot{T} - \nabla \cdot (\bm\lambda\nabla T - \sum_jh_jF_j) = h_s
\end{align}
$$
where $p$ and $T$ denote pore presure and temperature, while $\bm{\sigma}$ and $\bm{I}$ are the stress and identity tensors. $\alpha$ is the Biot’s constant, and $\beta$ is the thermal expansion coefficient for rocks, $\rho$ represents the density of porous media including both rock skeleton and filled fluids, and $\bm g$ the gravity vector. For the mass convervatiom equation, where $\bm u$ is the displacement vector of the solid phase, ${\bm \kappa}$ is the permeability tensor of porous media, $q$ is the fluid flow source, and $M$ is the Biot’s modulus. For the energy conservation equation,  $\bm\lambda$ is the thermal conductivity tensor for whole rock media, hj the specific enthalpy for phase j, 
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


### Initial and boundary conditions
For fluid flow field, we applied
* 101 $kPa$ atmospheric pore pressure on the top surface
* 34 $MPa$ pore pressure on the top surfaced
* Zero flux for all side surfaces


!media media/forge_bc.png
  id=fig:forgebc
  style=width:100%;
  caption= Illustration of boundary conditions for coupled three field.