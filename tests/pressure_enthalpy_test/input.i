################################################################################################################
# s_2012_test.i 
# Falcon input file for Stanford Geothermal Workshop 2012 simulations
# A 3D themo-poro-elastic example. small fracture network embedded in reservoir rock 
# IC is constant pressure, temperature/enthalpy, after ~4-5 ts of mechanics
#     *read in results of short transient simulation
# 500x250x250 m^3 reservoir
# initial reservoir pressure & temperature are 200 and 18.5MPa, respectively
# 
# This is a test for the source sink functionality
#   -no mechanics
#   -high heat capacity
#   -inject 150C water into a 200C reservoir
################################################################################################################

# heat extraction test only
[Mesh]
  dim = 3
  file = simple_3d_May_2012.e
[ ]

[Variables]
 active = 'pressure enthalpy'
 [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_condition = 20e6
  [../]

   [./enthalpy]
     order = FIRST
     family = LAGRANGE
     initial_condition = 850000
[../]
[ ]

[Preconditioning]
active = 'FDP'

[./FDP]
type = FDP
petsc_options =  '-snes_mf_operator -ksp_monitor'
petsc_options_iname = ' -pc_type -mat_fd_coloring_err -mat_fd_type'
petsc_options_value = ' lu 1.0e-9 ds'
#  off_diag_row    = 'pressure'
#  off_diag_column = 'enthalpy'
full = true
[../]
[ ]


[AuxVariables]
active = 'temperature viscosity_water density_water 
          v_x v_y v_z'

[./temperature]
order = FIRST
family = LAGRANGE
[../]

[./density_water]
order = CONSTANT
family = MONOMIAL
[../]

[./viscosity_water]
order = CONSTANT
family = MONOMIAL
[../]
[ ]

[./v_x]
 order = CONSTANT
 family = MONOMIAL
[../]

[./v_y]
 order = CONSTANT
 family = MONOMIAL
[../]

[./v_z]
 order = CONSTANT
 family = MONOMIAL
[../]
[ ]


[Kernels]
active = ' p_td p_wmfp p_wsfp t_td t_d t_cw t_cs'


[./p_td]
type = MassFluxTimeDerivative
variable = pressure
enthalpy= enthalpy
water_steam_properties = water_steam_properties
[../]

[./p_wmfp]
type = WaterMassFluxPressure
variable = pressure
enthalpy = enthalpy 
[../]

[./p_wsfp]
type = SteamMassFluxPressure
variable = pressure
enthalpy = enthalpy
[../]


# ----- energy conservation kernels ---------------------
[./t_td]
type = EnthalpyTimeDerivative
variable = enthalpy
temperature = temperature
pressure= pressure
water_steam_properties = water_steam_properties
[../]

[./t_d]
type = EnthalpyDiffusion
variable = enthalpy
pressure = pressure
temperature = temperature
[../]

[./t_cw]
type = EnthalpyConvectionWater
variable = enthalpy
pressure = pressure
[../]

[./t_cs]
type = EnthalpyConvectionSteam
variable = enthalpy
pressure = pressure
[../]


[ ]

[AuxKernels]
active = 'density_water temperature viscosity_water vx vy vz'

[./temperature]
type = CoupledTemperatureAux
variable = temperature
pressure = pressure
enthalpy = enthalpy
water_steam_properties = water_steam_properties
[../]

[./density_water]
type = MaterialRealAux
variable = density_water
property = density_water
[../]

[./viscosity_water]
type = MaterialRealAux
variable = viscosity_water
property = viscosity_water
[../]
[ ]

[./vx]
type = VelocityAux
variable = v_x
component = 0
[../]

[./vy]
type = VelocityAux
variable = v_y
component = 1
[../]

[./vz]
type = VelocityAux
variable = v_z
component = 2
[../]
[ ]



[BCs]
 active = 'left_p left_t right_p right_t'
[./left_p]
    type = DirichletBC
    variable = pressure
    boundary = '1'
    value = 20e6
 [../]	  

 [./left_t]
    type = DirichletBC
    variable = enthalpy
    value = 800000
    boundary = '1'
 [../]

[./right_p]
    type = DirichletBC
    variable = pressure
    boundary = '2'
    value = 19e6
 [../]

[./right_t]
    type = DirichletBC
    variable = enthalpy
    value = 850000
    boundary = '2'
 [../]
[ ]

[Materials]
 active = 'matrix'

[./matrix]
type = Geothermal
block = 1 

pressure = pressure
enthalpy = enthalpy
temperature = temperature
water_steam_properties = water_steam_properties
temp_dependent = false

gravity           =  0.0
gx                =  0.0
gy                =  0.0
gz                =  0.0

material_porosity    =  0.1

permeability         =  1e-15

density_rock         =  2500

thermal_conductivity =  2.5
specific_heat_water  = 4186
specific_heat_rock   =  920
[../]
[ ]


[UserObjects]
active = 'water_steam_properties'

[./water_steam_properties]
type = WaterSteamEOS
[../]
[ ]

 
[Executioner]
 active = ' '
 type =  Transient
petsc_options = '-snes_mf_operator'

 l_max_its  =  100
# l_tol =  1.0e-6
# l_abs_tol = 1e-9
# nl_max_its =  12
# nl_rel_tol =  1e-8
# nl_abs_tol = 1e-8

 
 dt = 1
 num_steps = 5

 [./Quadrature]
  type = Trap
 [../]
[ ]
 

[Output]
 file_base = out
 output_initial = true
 interval = 1
 output_variables = 'pressure temperature enthalpy v_x v_y v_z'
 exodus = true
 print_out_info = true
 [ ]
 
