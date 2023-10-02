# Effective Permeability and Thermal Conductivity Calibration Example using the Utah FORGE Native State Model

This example is prepared for the Year 1 Milestone 3.4.3 from the Utah FORGE project titled "Role of Fluid and Temperature in Fracture Mechanics and Coupled THMC Processes for Enhanced Geothermal Systems" (project number Purdue_5-2557).

## Problem Statement

This example uses the synthetic data set generated from the FORGE site model to infer the hydrogeological and geomechanical properties. In this initial study, the effective permeability and thermal conductivity values for pressure and temperature data are identified. Spatially distributed heterogeneous fields will be tested in the next updates. The details for the scalable site characterization can be found in [!cite](lee2018fast,kadeethum2021framework).

For the joint inversion example, 17,010 pressure, temperature, and displacement data points were created at the wells of 16A-32, 56-32, 58-32, 78-32, and 78B-32 from the native state model (Phase 3 updated in July 2022)[!cite](liu2022forge). A summary of the FALCON FORGE native statement model is available [here](examples/forge_falcon.md) with input and mesh available for down load from the [GDR website](https://gdr.openei.org/submissions/1397). A Gauss-Newton solver with a line search is used to find the effective parameters.

## Input File

The FORGE native state model assumes a two layer domain of isotropic and homogenous materials.  For testing purposes, the native state model was modified in this example to create a coarser 1000 element mesh using the following MOOSE [MeshGenerator.md]

!listing examples/forge_native_state_parameter_calibration/FORGE_NS_Ph3_coarse.i
         block=Mesh
         caption=[MeshGenerator.md] used for creating coarse FORGE native state model.

In order to increase the sensitity of the measurement data to changes in material parameters, the mesh was locally refined around the toe of well 78-32 using the following [Adaptivity](syntax/Adaptivity/index.md) block

!listing examples/forge_native_state_parameter_calibration/FORGE_NS_Ph3_coarse.i
         block=Adaptivity
         caption=Mesh refinement around the toe of well 78-32.

Material properties for the granitoid and sedimentary layers given in [!ref](examples/forge_falcon.md#tab:materialParams).  The thermal conductivity and permeability being calibrated for the granitoid layer are shown in the below material section of the input file:

!listing examples/forge_native_state_parameter_calibration/FORGE_NS_Ph3_coarse.i
         block=Materials/permeability_granite Materials/thermal_conductivity_granitoid
         caption=Materials being calibrated.
         id=input:materials

### Transient Simulation

Material calibration was performed using a transient simulation with a point source at the toe of well 78-32 (\[2327.3, 1795.9, 679.59\] in the native state model mesh coordinate). The injection rate was increased from 0 $kg/s$  to 0.1 $kg/s$ for the first 50 seconds and maintained at the same rate of 0.1 $kg/s$ until $t$ = 100 s. The injection temperature was constant at 323.15 $K$.

!listing examples/forge_native_state_parameter_calibration/FORGE_NS_Ph3_coarse.i
         block=DiracKernels
         caption=Point source at the toe of well 78-32

Pressure, temperature, and displacement data were extracted every 10s from well 16A-32, 56-32, 58-32, 78-32, and 78B-32. The simulation reached steady state at 70s.

## Results

The permeability and thermal conductivity of the granitoid layer were estimated in this case study. The initial log-permeability and log-conductivity were set to -16 and 1, respectively. With the Gauss-Newton solver, log-permeability and log-conductivity were optimized as follows:

\begin{equation}
K^{i+1}=K^{i}+\alpha \left( \sigma^{-1} + J^{T}_i R^{-1} J_i \right)^{-1} \left(\sigma^{-1}K^{i}+J^{T}_i R^{-1} \left[ y-g(K^{i}) \right] \right)
\end{equation}

where $K_{i}$ is the unknown variable log-permeabilty or log-conductivity at $i$-th iteration, $\sigma$ is the parameter error matrix, $R$ is the observation error matrix, $J_{i}$ is the Jacobian matrix, $y$ is the observed data, and $g(K^i)$ is the simulated data. $\alpha$ is the step length set to $0.5$ in this example.

!listing examples/forge_native_state_parameter_calibration/optimization_script.py
         caption=Script for effective parameter inversion.
         id=local

The input parameters in the python optimization script shown in [local] include the [MOOSE] input file line numbers for permeability and thermal conductivity shown [input:materials] which use zero based line numbers.

[fig:forge_effective_inv_iters] shows the estimation results of permeability and thermal conductivity for 8 iterations. [fig:forge_effective_inv_fitting] presents the pressure, temperature, and displacement fits, with the fitting error shown as root mean square error (RMSE).

!media media/forge_effective_inv_iters.png
  id=fig:forge_effective_inv_iters
  style=width:90%;
  caption= Estimation of permeability and thermal conductivity at each iteration

!media media/forge_effective_inv_fitting.png
  id=fig:forge_effective_inv_fitting
  style=width:90%;
  caption= Pressure, temperature and displacement fitting at the last iteration
