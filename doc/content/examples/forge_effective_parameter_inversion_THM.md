# Effective Permeability and Thermal Conductivity Calibration Example using the Utah FORGE Native State Model

This example is prepared for the Year 1 Milestone 3.4.3 from the Utah FORGE project titled "Role of Fluid and Temperature in Fracture Mechanics and Coupled THMC Processes for Enhanced Geothermal Systems" (project number Purdue_5-2557).

## Problem Statement 

This example uses the synthetic data set generated from the FORGE site model to infer the hydrogeological and geomechanical properties. In this initial study, the effective permeability and thermal conductivity values for pressure and temperature data are identified. Spatially distributed heterogeneous fields will be tested in the next updates. The details for the scalable site characterization can be found in [!cite](lee2018fast,kadeethum2021framework).

For the joint inversion example, 17,010 pressure, temperature, and displacement data points were created at the wells of 16A-32, 56-32, 58-32, 78-32, and 78B-32 from the native state model (Phase 3 updated in July 2022). Input files used here can be download from the [GDR website](https://gdr.openei.org/submissions/1397). A Gauss-Newton solver with a line search is used to find the effective parameters. 

## Input File

### Materials

[tab:materialParams] shows the material parameters, assuming that the materials of the two layers are isotropic and homogeneous. 

!table
  id=tab:materialParams
  caption=Material parameter values for observation generation from the FORGE native state simulation
| Parameter | Unit | Sedimentary Layer | Granitoid Layer |
| :-------: | :--: | :---------------: | :-------------: |
| Permeability | $m^2$ | 1e-14 | 1e-18 |
| Porosity | — | 0.12 | 0.001 |
| Grain density | $kg/m^3$ | 2500 | 2750 |
| Specific heat | $J/kg \cdot K$ | 830 | 790 |
| Thermal conductivity | $W/m \cdot K$ | 2.8 | 3.05 |
| Young’s modulus | $GPa$ | 30 | 62 |
| Poisson’s ratio | — | 0.3 | 0.3 |
| Biot Coefficient | — | 0.47 | 0.47 |
| Thermal expansion Coefficient | — | 6e-6 |6e-6 |

The material section of the input file is as follows:

```
[Materials]
#fluid properties and flow
	[./permeability_sediment]
		type = PorousFlowPermeabilityConst
		permeability = '1e-14 0 0  0 1e-14 0  0 0 1e-14'
		block = granitoid_40m_surface_40m
	[../]
	[./permeability_granite]
		type = PorousFlowPermeabilityConst
		permeability = '1e-18 0 0  0 1e-18 0  0 0 1e-18'
		block = bottom_40m_granitoid_40m
	[../]
	[./fluid_props]
		type = PorousFlowSingleComponentFluid
		fp = tabulated_water
		phase = 0
		at_nodes = true
	[../]
	[./fluid_props_qp]
		type = PorousFlowSingleComponentFluid
		phase = 0
		fp = tabulated_water
	[../]
	[./porosity_sediment]
		type = PorousFlowPorosityConst
		porosity = 0.12
		block = granitoid_40m_surface_40m
	[../]
	[./porosity_granite]
		type = PorousFlowPorosityConst
		porosity = 0.001
		block = bottom_40m_granitoid_40m
	[../]
	[./ppss]
		type = PorousFlow1PhaseP
		at_nodes = true
		porepressure = pressure
		capillary_pressure = pc
	[../]
	[./ppss_qp]
		type = PorousFlow1PhaseP
		porepressure = pressure
		capillary_pressure = pc
	[../]
	[./massfrac]
		type = PorousFlowMassFraction
		at_nodes = true
	[../]
	[./eff_fluid_pressure]
		type = PorousFlowEffectiveFluidPressure
		at_nodes = true
	[../]
	[./eff_fluid_pressure_qp]
		type = PorousFlowEffectiveFluidPressure
	[../]
	[./relperm]
		type = PorousFlowRelativePermeabilityCorey
		at_nodes = true
		n = 1
		phase = 0
	[../]
	#energy transport
	[./temperature]
		type = PorousFlowTemperature
		temperature = temperature
	[../]
	[./temperature_nodal]
		type = PorousFlowTemperature
		at_nodes = true
		temperature = temperature
	[../]
	[./rock_heat_sediments]
		type = PorousFlowMatrixInternalEnergy
		specific_heat_capacity = 830.0
		density = 2500
		block = granitoid_40m_surface_40m
		#density from Rick allis analysis, specific heat per tabulated values
	[../]
	[./rock_heat_granitoid]
		type = PorousFlowMatrixInternalEnergy
		specific_heat_capacity = 790.0
		density = 2750
		block = bottom_40m_granitoid_40m
		#density from Rick allis analysis, specific heat per tabulated values
	[../]
	[./thermal_conductivity_sediments]
		type = PorousFlowThermalConductivityIdeal
		dry_thermal_conductivity = '2.8 0 0  0 2.8 0  0 0 2.8'
		block = granitoid_40m_surface_40m
	[../]
	[./thermal_conductivity_granitoid]
		type = PorousFlowThermalConductivityIdeal
		dry_thermal_conductivity = '3.05 0 0  0 3.05 0  0 0 3.05'
		block = bottom_40m_granitoid_40m
	[../]
	#mechanics
	[./density_sediment]
		type = GenericConstantMaterial
		prop_names = density
		block = granitoid_40m_surface_40m
		prop_values = 2500.0
	[../]
	[./density_granitoid]
		type = GenericConstantMaterial
		prop_names = density
		block = bottom_40m_granitoid_40m
		prop_values = 2750.0
	[../]
	[./biot]
	type = GenericConstantMaterial
	prop_names = biot_coefficient
	prop_values = 0.47
	[../]
	[./elasticity_sediment]
	type = ComputeIsotropicElasticityTensor
	youngs_modulus = 3.0e+10
	poissons_ratio = 0.30
	block = granitoid_40m_surface_40m
	[../]
	[./elasticity_granitoid]
	type = ComputeIsotropicElasticityTensor
	youngs_modulus = 6.2e+10
	poissons_ratio = 0.3
	block = bottom_40m_granitoid_40m
	[../]
	[./thermal_expansion_strain]
		type = ComputeThermalExpansionEigenstrain
		stress_free_temperature = 293
		thermal_expansion_coeff = 6.0E-6
		temperature = temperature
		eigenstrain_name = eigenstrain
	[../]
	[./strain]
	type = ComputeSmallStrain
	[../]
	[./stress]
	type = ComputeLinearElasticStress
	[../]
[]
```

### Initial and Boundary Conditions

#### For the solid field, the boundary conditions are:

- fixed displacement of the bottom and three side surfaces
- z coordinate dependent functions applied for the normal traction of the right surface and shear traction of the right and left surfaces
- constant traction applied to the top and bottom surfaces

```
# B.C. for solid field
# Prescribed displacements at three boundary surfaces
[./roller_sigma_h_min_x_minus]
	type = DirichletBC
	preset = true
	variable = disp_i
	value = 0
	boundary = 'bottom_40m_granitoid_40m_front granitoid_40m_surface_40m_front'
[../]
[./roller_sigma_h_min_x_plus]
	type = DirichletBC
	preset = true
	variable = disp_i
	value = 0
	boundary = 'bottom_40m_granitoid_40m_back granitoid_40m_surface_40m_back'
[../]	
[./roller_sigma_h_max_minus]
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
# Prescribed traction
[./S_h_max_normal_righ]
	type = FunctionNeumannBC
	variable = disp_j
	boundary = 'bottom_40m_granitoid_40m_left granitoid_40m_surface_40m_left'
	function  = '-19889*(1785-z)'
[../]	
[./S_h_max_shear_right]
	type = FunctionNeumannBC
	variable = disp_k
	boundary = 'bottom_40m_granitoid_40m_left granitoid_40m_surface_40m_left'
	function  = '929*(1785-z)'
[../]	
[./S_h_max_shear_left]
	type = FunctionNeumannBC
	variable = disp_k
	boundary = 'bottom_40m_granitoid_40m_right granitoid_40m_surface_40m_right'
	function  = '-929*(1785-z)'
[../]	
[./Side_Z_minus_traction_Z]
	type = NeumannBC
	variable = disp_j
	boundary = 'bottom_40m'
	value = -3.85e+6
[../]	
[./Side_Z_Plus_traction_Z]
	type = NeumannBC
	variable = disp_k
	boundary = 'surface_40m'
	value = -101325
[../] 
```

#### For the fluid flow field, the boundary conditions are:

- atmospheric pore pressure on the top surface
- 34 $MPa$ pore pressure on the bottom surface

```
#B.C. for Darcy's flow field
# Apply zero pore pressure on top surface
# and no flow side surfaces
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

#### For heat transfer, the boundary conditions are:

- 299 $K$ temperature on the top surface
- distributed temperature on the bottom surface using a defined function

```
#B.C. Temperature field
# Apply prescibed temperature on top and bottom surfaces
# and no heat flux on the side surfaces
[./Side_Z_Plus_T]
	type = DirichletBC
	variable = temperature
	boundary = 'surface_40m'
	value= 299
[../]
[./T_on_face_Z_Minus]
	type = PiecewiseBilinear
	data_file = FORGE_bottom_2021.11.10_plus_50.csv
	xaxis = 1
	yaxis = 0
[../]
[./Side_Z_Minus_T]
	type = FunctionDirichletBC
	#type = DirichletBC
	variable = temperature
	boundary = 'bottom_40m'
	function = T_on_face_Z_Minus
[../]
```
#### Initial Condition

The pore pressure and temperature were initialized using the z dependent functions:

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

### Transient Simulation

Transient simulation was performed by setting a point source at the toe of well 78-32 (\[2327.3, 1795.9, 679.59\] in the native state model mesh coordinate). The injection rate was increased from 0 $kg/s$  to 0.1 $kg/s$ for the first 50 seconds and maintained at the same rate of 0.1 $kg/s$ until $t$ = 100 s. The injection temperature was constant at 323.15 $K$.

```
[mass_flux_in_fn]
	type = PiecewiseLinear
	xy_data = '
	0   0.0
	50  0.1
	100  0.1'
[]
[T_in_fn]
	type = PiecewiseLinear
	xy_data = '
	0   323.15
	50   323.15
	100  323.15'
[]
[source]
	type = PorousFlowPointSourceFromPostprocessor
	variable = pressure
	mass_flux = mass_flux_in
	point = '2327.3 1795.9 679.59'
[]
[source_h]
	type = PorousFlowPointEnthalpySourceFromPostprocessor
	variable = temperature
	mass_flux = mass_flux_in
	point = '2327.3 1795.9 679.59'
	T_in = T_in
	pressure = pressure
	fp = true_water
[]
```

Pressure, temperature, and displacement data were extracted every 10s from well 16A-32, 56-32, 58-32, 78-32, and 78B-32. The simulation reached steady state at 70s.

```
[Executioner]
	line_search = None
	type = Transient
	solve_type = NEWTON

	steady_state_detection = true
	steady_state_tolerance = 1e-8

	dt = 10
	end_time = 100
	start_time = 0

	l_max_its  = 1000
	nl_max_its = 100
	l_tol      = 1e-6
	nl_abs_tol = 1E-8
	nl_rel_tol = 1e-6
[]
```

## Results

The permeability and thermal conductivity of the granitoid layer were estimated in this case study. The initial log-permeability and log-conductivity were set to -16 and 1, respectively. With the Gauss-Newton solver, log-permeability and log-conductivity were optimized as follows:

\begin{equation}
K^{i+1}=K^{i}+\alpha \left( \sigma^{-1} + J^{T}_i R^{-1} J_i \right)^{-1} \left(\sigma^{-1}K^{i}+J^{T}_i R^{-1} \left[ y-g(K^{i}) \right] \right)
\end{equation}

where $K_{i}$ is the unknown variable log-permeabilty or log-conductivity at $i$-th iteration, $\sigma$ is the parameter error matrix, $R$ is the observation error matrix, $J_{i}$ is the Jacobian matrix, $y$ is the observed data, and $g(K^i)$ is the simulated data. $\alpha$ is the step length set to $0.5$ in this example. 

!listing! id=local caption=Script for effective parameter inversion. language=python
##
## data used in this example comes from GDR repository https://gdr.openei.org/submissions/1397 
##

import os
import time
import argparse
import pandas as pd
import numpy as np

parser = argparse.ArgumentParser()
parser.add_argument('--falcon_input', default='FORGE_NS_Ph3_2022.07.25.i', help='input file of the native state model')
parser.add_argument('--falcon_run', default='../falcon/falcon-opt', help='path to falcon')
parser.add_argument('--n_cores', type=int, default = 48, help='cpu cores')
parser.add_argument('--max_iter', type=int, default = 10, help='max iterations')
parser.add_argument('--nK', type=int, default = 2, help='number of parameters')
parser.add_argument('--line_perm', type=int, default = 495, help='line number of permeability in the input file')
parser.add_argument('--line_cond', type=int, default = 594, help='line number of thermal conductivity in the input file')
parser.add_argument('--precision', type=float, default = 1.e-5, help='precision')
parser.add_argument('--alpha', type=float, default = 0.5, help='line search step length')
parser.add_argument('--K_Perm', type=float, default = -16.0, help='initial permeability')
parser.add_argument('--K_Cond', type=float, default = 1.0, help='initial thermal conductivity')
parser.add_argument('--folder_ref', default='./Ref/', help="path to reference data")
parser.add_argument('--outf', default='./Results', help="output folder")
par = parser.parse_args()

outf = par.outf
data_f = outf + '/data'    # output data
try:
    os.makedirs(data_f)
except OSError:
    pass

falcon_input = par.falcon_input
falcon_run = par.falcon_run
n_cores = par.n_cores
max_iter = int(par.max_iter)
nK = par.nK
line_perm = par.line_perm
line_cond = par.line_cond
folder_ref = par.folder_ref    # reference data 
folder_simul = './'      # simulated data
num_file = 7      # number of simulation time steps

## Define a function to read data
def Read_data(folder,num_file):
    # data comes from GDR repository https://gdr.openei.org/submissions/1397
    well_16A = 'FORGE_NS_Ph3_2022.07.25_point_data_16A_'
    well_56 = 'FORGE_NS_Ph3_2022.07.25_point_data_56_32_'
    well_58 = 'FORGE_NS_Ph3_2022.07.25_point_data_58_32_'
    well_78 = 'FORGE_NS_Ph3_2022.07.25_point_data_78_32_'
    well_78B = 'FORGE_NS_Ph3_2022.07.25_point_data_78B_32_'
    
    data_pressure_16A, data_temper_16A, data_dispi_16A, data_dispj_16A, data_dispk_16A = [], [], [], [], []
    data_pressure_56, data_temper_56, data_dispi_56, data_dispj_56, data_dispk_56 = [], [], [], [], []
    data_pressure_58, data_temper_58, data_dispi_58, data_dispj_58, data_dispk_58 = [], [], [], [], []
    data_pressure_78, data_temper_78, data_dispi_78, data_dispj_78, data_dispk_78 = [], [], [], [], []
    data_pressure_78B, data_temper_78B, data_dispi_78B, data_dispj_78B, data_dispk_78B = [], [], [], [], []

    for i in range(1,num_file+1):
        string = str(i)
        str_16A = well_16A + string.zfill(4) + '.csv'
        str_56 = well_56 + string.zfill(4) + '.csv'
        str_58 = well_58 + string.zfill(4) + '.csv'
        str_78 = well_78 + string.zfill(4) + '.csv'
        str_78B = well_78B + string.zfill(4) + '.csv'
    
        pressure_16A = np.array( pd.read_csv(folder+str_16A,usecols=['pressure']) )
        temper_16A = np.array( pd.read_csv(folder+str_16A,usecols=['temperature']) )
        dispi_16A = np.array( pd.read_csv(folder+str_16A,usecols=['disp_i']) )
        dispj_16A = np.array( pd.read_csv(folder+str_16A,usecols=['disp_j']) )
        dispk_16A = np.array( pd.read_csv(folder+str_16A,usecols=['disp_k']) )
        
        pressure_56 = np.array( pd.read_csv(folder+str_56,usecols=['pressure']) )
        temper_56 = np.array( pd.read_csv(folder+str_56,usecols=['temperature']) )
        dispi_56 = np.array( pd.read_csv(folder+str_56,usecols=['disp_i']) )
        dispj_56 = np.array( pd.read_csv(folder+str_56,usecols=['disp_j']) )
        dispk_56 = np.array( pd.read_csv(folder+str_56,usecols=['disp_k']) )
    
        pressure_58 = np.array( pd.read_csv(folder+str_58,usecols=['pressure']) )
        temper_58  = np.array( pd.read_csv(folder+str_58,usecols=['temperature']) )
        dispi_58  = np.array( pd.read_csv(folder+str_58,usecols=['disp_i']) )
        dispj_58  = np.array( pd.read_csv(folder+str_58,usecols=['disp_j']) )
        dispk_58  = np.array( pd.read_csv(folder+str_58,usecols=['disp_k']) )

        pressure_78 = np.array( pd.read_csv(folder+str_78,usecols=['pressure']) )
        temper_78 = np.array( pd.read_csv(folder+str_78,usecols=['temperature']) )
        dispi_78 = np.array( pd.read_csv(folder+str_78,usecols=['disp_i']) )
        dispj_78 = np.array( pd.read_csv(folder+str_78,usecols=['disp_j']) )
        dispk_78 = np.array( pd.read_csv(folder+str_78,usecols=['disp_k']) )

        pressure_78B = np.array( pd.read_csv(folder+str_78B,usecols=['pressure']) )
        temper_78B = np.array( pd.read_csv(folder+str_78B,usecols=['temperature']) )
        dispi_78B = np.array( pd.read_csv(folder+str_78B,usecols=['disp_i']) )
        dispj_78B = np.array( pd.read_csv(folder+str_78B,usecols=['disp_j']) )
        dispk_78B = np.array( pd.read_csv(folder+str_78B,usecols=['disp_k']) )
        
        data_pressure_16A.append(pressure_16A), data_temper_16A.append(temper_16A)
        data_dispi_16A.append(dispi_16A), data_dispj_16A.append(dispj_16A), data_dispk_16A.append(dispk_16A)
            
        data_pressure_56.append(pressure_56), data_temper_56.append(temper_56)
        data_dispi_56.append(dispi_56), data_dispj_56.append(dispj_56), data_dispk_56.append(dispk_56)
        
        data_pressure_58.append(pressure_58), data_temper_58.append(temper_58)
        data_dispi_58.append(dispi_58), data_dispj_58.append(dispj_58), data_dispk_58.append(dispk_58)
        
        data_pressure_78.append(pressure_78), data_temper_78.append(temper_78)
        data_dispi_78.append(dispi_78), data_dispj_78.append(dispj_78), data_dispk_78.append(dispk_78)
        
        data_pressure_78B.append(pressure_78B), data_temper_78B.append(temper_78B)
        data_dispi_78B.append(dispi_78B), data_dispj_78B.append(dispj_78B), data_dispk_78B.append(dispk_78B)

    data_pressure_16A = np.array(data_pressure_16A).reshape(-1,1) 
    data_temper_16A = np.array(data_temper_16A).reshape(-1,1)
    data_dispi_16A = np.array(data_dispi_16A).reshape(-1,1)
    data_dispj_16A = np.array(data_dispj_16A).reshape(-1,1)
    data_dispk_16A = np.array(data_dispk_16A).reshape(-1,1)

    data_pressure_56 = np.array(data_pressure_56).reshape(-1,1) 
    data_temper_56 = np.array(data_temper_56).reshape(-1,1)
    data_dispi_56 = np.array(data_dispi_56).reshape(-1,1)
    data_dispj_56 = np.array(data_dispj_56).reshape(-1,1)
    data_dispk_56 = np.array(data_dispk_56).reshape(-1,1)

    data_pressure_58 = np.array(data_pressure_58).reshape(-1,1) 
    data_temper_58 = np.array(data_temper_58).reshape(-1,1)
    data_dispi_58 = np.array(data_dispi_58).reshape(-1,1)
    data_dispj_58 = np.array(data_dispj_58).reshape(-1,1)
    data_dispk_58 = np.array(data_dispk_58).reshape(-1,1)

    data_pressure_78 = np.array(data_pressure_78).reshape(-1,1) 
    data_temper_78 = np.array(data_temper_78).reshape(-1,1)
    data_dispi_78 = np.array(data_dispi_78).reshape(-1,1)
    data_dispj_78 = np.array(data_dispj_78).reshape(-1,1)
    data_dispk_78 = np.array(data_dispk_78).reshape(-1,1)

    data_pressure_78B = np.array(data_pressure_78B).reshape(-1,1) 
    data_temper_78B = np.array(data_temper_78B).reshape(-1,1)
    data_dispi_78B = np.array(data_dispi_78B).reshape(-1,1)
    data_dispj_78B = np.array(data_dispj_78B).reshape(-1,1)
    data_dispk_78B = np.array(data_dispk_78B).reshape(-1,1)           
    
    pressure = np.concatenate((data_pressure_16A,data_pressure_56,data_pressure_58,data_pressure_78,data_pressure_78B),axis=0)
    temper = np.concatenate((data_temper_16A,data_temper_56,data_temper_58,data_temper_78,data_temper_78B),axis=0)
    dispi = np.concatenate((data_dispi_16A,data_dispi_56,data_dispi_58,data_dispi_78,data_dispi_78B),axis=0)
    dispj = np.concatenate((data_dispj_16A,data_dispj_56,data_dispj_58,data_dispj_78,data_dispj_78B),axis=0)
    dispk = np.concatenate((data_dispk_16A,data_dispk_56,data_dispk_58,data_dispk_78,data_dispk_78B),axis=0)
    data = np.concatenate((pressure,temper,dispi,dispj,dispk),axis=0)
    
    return data

## Forward modeling function
def Forward(K_log):
    K_Perm = K_log[0]
    K_Cond = K_log[1]
    
    K_Perm = np.power(10,K_Perm)
    K_Cond = np.power(10,K_Cond)

    K_Perm = np.format_float_scientific(K_Perm)
    K_Cond = float(K_Cond)
    
    inputf = open(falcon_input,'r+')
    info = inputf.readlines()
    inputf.close()
    
    # rewrite permeability
    write1 = info[line_perm].split() 
    write1[2] = '\'' + str(K_Perm) 
    write1[6] = str(K_Perm) 
    write1[10] = str(K_Perm) + '\''
    newstr1 = ' '.join(write1)
    info[line_perm] = ' '*4 + newstr1 + '\n'
    
    # rewrite thermal conductivity
    write2 = info[line_cond].split()
    write2[2] = '\'' + str(K_Cond) 
    write2[6] = str(K_Cond) 
    write2[10] = str(K_Cond) + '\''
    newstr2 = ' '.join(write2)
    info[line_cond] = ' '*4 + newstr2 + '\n'
    
    inputf = open(falcon_input,'w+')
    inputf.writelines(info)
    inputf.close()
    
    # run simulations
    run_command = 'mpiexec -n'+' '+str(n_cores)+' '+falcon_run+' '+ '-i' \
                  +' '+falcon_input+' '+'--n-threads=2 --timing'
    os.system(run_command)
    simul = Read_data(folder=folder_simul,num_file=num_file)

    return simul

## Delete data produced by previous simulations
def remove_data(folder):
    filePath = folder
    name = os.listdir(filePath)
    for i in name:
        path = './{}'.format(i)
        if 'point_data' in i:
            os.remove(path)

## Assume known error matrix R
obs_true = Read_data(folder=folder_ref,num_file=num_file)
np.save(data_f+'/Ref_Data.npy', obs_true)
nobs = len(obs_true)
std_pressure = obs_true[0] * 0.01   # 1% std error
std_temper = obs_true[int(nobs/5)] * 0.01   # 1% std error
std_dispi = obs_true[int(2*nobs/5)] * 0.01   # 1% std error
std_dispj = obs_true[int(3*nobs/5)] * 0.01   # 1% std error
std_dispk = obs_true[int(4*nobs/5)] * 0.01   # 1% std error
std_pressure = np.abs(std_pressure)
std_temper = np.abs(std_temper)
std_dispi = np.abs(std_dispi)
std_dispj = np.abs(std_dispj)
std_dispk = np.abs(std_dispk)

err_pressure = std_pressure*np.random.randn(int(nobs/5),1)
err_temper = std_temper*np.random.randn(int(nobs/5),1)
err_dispi = std_dispi*np.random.randn(int(nobs/5),1)
err_dispj = std_dispj*np.random.randn(int(nobs/5),1)
err_dispk = std_dispk*np.random.randn(int(nobs/5),1)
err = np.concatenate((err_pressure,err_temper,err_dispi,err_dispj,err_dispk),axis=0)
obs = obs_true + err

invR = np.eye(nobs)
for p in range(int(nobs/5)):
    invR[p,p] = 1/(std_pressure**2)
for t in range(int(nobs/5),int(2*nobs/5)):
    invR[t,t] = 1/(std_temper**2)
for i in range(int(2*nobs/5),int(3*nobs/5)):
    invR[i,i] = 1/(std_dispi**2)
for j in range(int(3*nobs/5),int(4*nobs/5)):
    invR[j,j] = 1/(std_dispj**2)
for k in range(int(4*nobs/5),nobs):
    invR[k,k] = 1/(std_dispk**2)

## Initialization
K_Perm = par.K_Perm
K_Cond = par.K_Cond
K_init = np.vstack((K_Perm,K_Cond))   # initial parameters  

std_Perm = 2   
std_Cond = 0.05  
std_Par = np.vstack((std_Perm,std_Cond))
Sigma = np.eye(nK)
invSigma = np.eye(nK)
for i in range(nK):
    Sigma[i,i] = std_Par[i]**2
    invSigma[i,i] = 1/Sigma[i,i]

precision = par.precision
alpha = par.alpha
K_cur = np.copy(K_init)
Para = np.zeros([max_iter+1,nK])

## Run inversion
Time_start = time.time()
for i in range(max_iter):
    print('###### Iteration %d ######' % (i+1))
    remove_data(folder='./')
    simul_K_cur = Forward(K_cur)
    
    Para[i,0] = K_cur[0]
    Para[i,1:nK] = K_cur[1:nK].squeeze()
    np.save(data_f+'/Simul_Data_Iter_'+str(i)+'.npy', simul_K_cur)
    np.save(data_f+'/Parameters.npy', Para)
    
    J_cur = np.zeros([nobs,nK])
    for j in range(nK):
        zerovec = np.zeros([nK,1])
        zerovec[j] = 1.
        mag = np.dot(K_cur.T,zerovec)
        absmag = np.dot(abs(K_cur.T),abs(zerovec))
        if mag >= 0:
            signmag = 1.
        else:
            signmag = -1.
    
        delta = signmag*np.sqrt(precision)*(max(abs(mag),absmag))/((np.linalg.norm(zerovec)+ np.finfo(float).eps)**2)
        if delta == 0:
            delta = np.sqrt(precision)
  
        remove_data(folder='./')
        simul_K_delta = Forward(K_cur+zerovec*delta)
        J_cur[:,j:j+1] = (simul_K_delta - simul_K_cur)/delta

    Jk_cur = np.dot(J_cur,K_cur)
    solve_a = np.dot( np.dot(J_cur.T,invR),J_cur) + invSigma 
    solve_b = -np.dot(invSigma,K_cur) + np.dot( np.dot(J_cur.T,invR), (obs - simul_K_cur) )
    dk = np.linalg.solve(solve_a,solve_b)
    K_cur = np.copy(K_cur + alpha*dk)

## Final results
remove_data(folder='./')
simul_K_cur = Forward(K_cur)

Para[max_iter,0] = K_cur[0]
Para[max_iter,1:nK] = K_cur[1:nK].squeeze()
np.save(data_f+'/Simul_Data_Iter_'+str(max_iter)+'.npy', simul_K_cur)
np.save(data_f+'/Parameters.npy', Para)

Time_end = time.time()
time_elapsed = Time_end - Time_start
print('Time elapsed {:.0f}hr {:.0f}m {:.0f}s'.format( 
    (time_elapsed // 60) // 60,
    (time_elapsed // 60) % 60, 
    time_elapsed % 60) 
    ) 
!listing-end!


[fig:forge_effective_inv_iters] shows the estimation results of permeability and thermal conductivity for 8 iterations. [fig:forge_effective_inv_fitting] presents the pressure, temperature, and displacement fits, with the fitting error shown as root mean square error (RMSE).

!media media/forge_effective_inv_iters.png
  id=fig:forge_effective_inv_iters
  style=width:90%;
  caption= Estimation of permeability and thermal conductivity at each iteration

!media media/forge_effective_inv_fitting.png
  id=fig:forge_effective_inv_fitting
  style=width:90%;
  caption= Pressure, temperature and displacement fitting at the last iteration
