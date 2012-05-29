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
     initial_condition = 0.85
[../]
[ ]

[Preconditioning]
active = ' '
[ ]


[AuxVariables]
active = 'density density_water density_steam enthalpy_water enthalpy_steam
          ddensitydH_P ddensitydp_H denthalpy_steamdH_P denthalpy_waterdH_P
          saturation_water dswdH dTdH_P temperature 
          viscosity_water viscosity_steam 
          denthalpy_waterdP_H denthalpy_steamdP_H dTdP_H
          v_x v_y v_z'

[./density]
order = FIRST
family = LAGRANGE
initial_condition = 878.98804480291119
[../]

[./density_water]
order = FIRST
family = LAGRANGE
#initial_condition = 0
[../]

[./density_steam]
order = FIRST
family = LAGRANGE
#initial_condition = 878.98804480291119
[../]

[./viscosity_water]
order = FIRST
family = LAGRANGE
#initial_condition = 1.81312661535894764E-004
[../]

[./viscosity_steam]
order = FIRST
family = LAGRANGE
#initial_condition = 1.34E-5
[../]

[./enthalpy_water]
order = FIRST
family = LAGRANGE
initial_condition = 0
[../]

[./enthalpy_steam]
order = FIRST
family = LAGRANGE
#initial_condition = 878.98804480291119
[../]

[./saturation_water]
order = FIRST
family = LAGRANGE
initial_condition = 1.
[../]

[./temperature]
order = FIRST
family = LAGRANGE
initial_condition = 878.98804480291119
[../]

[./ddensitydH_P]
order = FIRST
family = LAGRANGE
[../]

[./ddensitydp_H]
order = FIRST
family = LAGRANGE
[../]

[./denthalpy_steamdH_P]
order = FIRST
family = LAGRANGE
[../]

[./denthalpy_waterdH_P]
order = FIRST
family = LAGRANGE
[../]

[./denthalpy_waterdP_H]
order = FIRST
family = LAGRANGE
[../]

[./denthalpy_steamdP_H]
order = FIRST
family = LAGRANGE
[../]

[./dTdP_H]
order = FIRST
family = LAGRANGE
[../]

[./dswdH]
order = FIRST
family = LAGRANGE
[../]

[./dTdH_P]
order = FIRST
family = LAGRANGE
[../]

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
density = density
ddensitydp_H = ddensitydp_H
enthalpy= enthalpy
ddensitydH_P = ddensitydH_P
[../]

[./p_wmfp]
type = WaterMassFluxPressure
variable = pressure
enthalpy = enthalpy 
pressure = pressure
[../]

[./p_wsfp]
type = SteamMassFluxPressure
variable = pressure
enthalpy = enthalpy
pressure = pressure
[../]


# ----- energy conservation kernels ---------------------
[./t_td]
type = EnthalpyTimeDerivative
variable = enthalpy
density = density
temperature = temperature
dTdH_P = dTdH_P
dTdP_H = dTdP_H
ddensitydH_P = ddensitydH_P
pressure= pressure
ddensitydp_H = ddensitydp_H
[../]

[./t_d]
type = EnthalpyDiffusion
variable = enthalpy
pressure = pressure
temperature = temperature
dTdH_P = dTdH_P
dTdP_H = dTdP_H
[../]

[./t_cw]
type = EnthalpyConvectionWater
variable = enthalpy
enthalpy_water = enthalpy_water
denthalpy_waterdH_P = denthalpy_waterdH_P
denthalpy_waterdP_H = denthalpy_waterdP_H
# ddensity_waterdP_H = ddensity_waterdP_H
pressure = pressure
viscosity_water = viscosity_water
saturation_water = saturation_water
[../]

[./t_cs]
type = EnthalpyConvectionSteam
variable = enthalpy
enthalpy_steam = enthalpy_steam
denthalpy_steamdH_P = denthalpy_steamdH_P
denthalpy_steamdP_H = denthalpy_steamdP_H
# ddensity_steamdP_H = ddensity_steamdP_H
pressure = pressure
visocitysteam = viscosity_steam
saturation_water = saturation_water
[../]


[ ]

[AuxKernels]
active = 'density density_water density_steam enthalpy_water enthalpy_steam
          ddensitydH_P ddensitydp_H denthalpy_steamdH_P denthalpy_waterdH_P
          saturation_water dswdH dTdH_P temperature 
          viscosity_water viscosity_steam 
          denthalpy_waterdP_H denthalpy_steamdP_H dTdP_H
          vx vy vz'

[./density]
type = CoupledDensityAux
variable = density
density = 891.89268782189379
[../]

[./density_water]
type = CoupledWaterDensityAux
variable = density_water
density_water = 878.98804480291119
[../]

[./density_steam]
type = CoupledSteamDensityAux
variable = density_steam
density_steam =0.0
[../]

[./enthalpy_water] 
type = CoupledWaterEnthalpyAux
variable = enthalpy_water
enthalpy_water = 799109.07281387434e-6
[../]

[./enthalpy_steam] 
type = CoupledSteamEnthalpyAux
variable = enthalpy_steam
enthalpy_steam = 0e0
[../]

#  ------------   end of properties -------

[./ddensitydH_P]
type = CoupledDdensityDHAux
variable = ddensitydH_P
[../]

[./ddensitydp_H]
type =  CoupledDdensityDPAux
variable = ddensitydp_H
[../]


[./denthalpy_steamdH_P]
type = CoupledDsteamenthalpydH_PAux
variable =  denthalpy_steamdH_P
[../]

[./denthalpy_waterdH_P]
type = CoupledDwaterenthalpydH_PAux
variable = denthalpy_waterdH_P
[../]


[./denthalpy_steamdP_H]
type = CoupledDsteamenthalpydP_HAux
variable =  denthalpy_steamdP_H
[../]

[./denthalpy_waterdP_H]
type = CoupledDwaterenthalpydP_HAux
variable = denthalpy_waterdP_H
[../]

[./dTdP_H]
type = CoupledDTDH_PAux
variable = dTdP_H
[../]


[./saturation_water]
type = CoupledWaterSaturationAux
variable = saturation_water
saturation_water = 1.0
[../]

[./dswdH]
type = CoupledDWaterSaturationDHAux
variable = dswdH
[../]

[./dTdH_P]
type = CoupledDTDH_PAux
variable = dTdH_P
[../]

[./temperature]
type = CoupledTemperatureAux
variable = temperature
pressure = pressure
enthalpy = enthalpy
saturation_water = saturation_water
density = density
density_water = density_water
density_steam = density_steam
enthalpy_water = enthalpy_water
enthalpy_steam = enthalpy_steam
ddensitydH_P = ddensitydH_P
ddensitydp_H = ddensitydp_H
denthalpy_waterdH_P = denthalpy_waterdH_P
denthalpy_steamdH_P = denthalpy_steamdH_P
dTdH_P = dTdH_P
dswdH = dswdH
ddensity_waterdP_H = ddensity_waterdP_H
ddensity_steamdP_H = ddensity_steamdP_H
denthalpy_waterdP_H = denthalpy_waterdP_H
denthalpy_steamdP_H = denthalpy_steamdP_H
dTdP_H = dTdP_H
# temperature =188.0
[../]

# ------------------------------------------------------

[./viscosity_water]
type = CoupledSteamViscosityAux
variable = viscosity_water
density_steam = density_water
temperature = temperature
viscosity_water = 1.81312661535894764E-004
temp_dependent_viscosity = true
# type = CoupledWaterViscosityAux
# variable = viscosity_water
# temperature = temperature
# pressure = pressure
# viscosity_water = 1.81312661535894764E-004
# temp_dependent_viscosity = true
[../]

[./viscosity_steam]
type = CoupledSteamViscosityAux
variable = viscosity_steam
density_steam = density_steam
temperature = temperature
viscosity_steam =1.35765463729880601E-005
temp_dependent_viscosity = true
[../]


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
	pressure = pressure
	temperature = temperature
	dTdH_P = dTdH_P
	dTdP_H = dTdP_H
	enthalpy_water = enthalpy_water
	enthalpy_steam = enthalpy_steam
	denthalpy_waterdH_P = denthalpy_waterdH_P
	denthalpy_steamdH_P = denthalpy_steamdH_P
	denthalpy_waterdP_H = denthalpy_waterdP_H
	denthalpy_steamdP_H = denthalpy_steamdP_H
    value = 0.8
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
	pressure = pressure
	temperature = temperature
	dTdH_P = dTdH_P
	dTdP_H = dTdP_H
	enthalpy_water = enthalpy_water
	enthalpy_steam = enthalpy_steam
	denthalpy_waterdH_P = denthalpy_waterdH_P
	denthalpy_steamdH_P = denthalpy_steamdH_P
	denthalpy_waterdP_H = denthalpy_waterdP_H
	denthalpy_steamdP_H = denthalpy_steamdP_H
    value = 0.85
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
density_water = density_water
viscosity_water = viscosity_water

gravity           =  0.0
gx                =  0.0
gy                =  0.0
gz                =  0.0

material_porosity    =  0.1

permeability         =  1e-15

density_rock         =  2500

thermal_conductivity =  2.5e-6
specific_heat_water  = 4186e-6
specific_heat_rock   =  920e-6
[../]


[ ]
 
[Executioner]
 active = ' '
 type =  Transient

# l_max_its  =  100
# l_tol =  1.0e-6
# l_abs_tol = 1e-9
# nl_max_its =  12
# nl_rel_tol =  1e-5
# nl_abs_tol = 1e-8

 
 dt = 1
 num_steps = 25

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
 
