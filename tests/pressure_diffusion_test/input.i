[Mesh]
  file = simple_3d_May_2012.e
[]

[Variables]
  active = 'pressure'
  [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_condition = 10e6
  [../]

  [./temperature]
   order = FIRST
   family = LAGRANGE
   initial_condition = 200.0
  [../]
[]


[AuxVariables]
#active = 'v_x'
 active = ' ' 

[./v_x]
 order = CONSTANT
 family = MONOMIAL
[../]

[]

[Kernels]
active = ' p_wmfp'

 [./p_wmfp]
 type = WaterMassFluxPressure_PT
 variable = pressure
 [../]

[]

[AuxKernels]
#active = 'vx'
 active = ' '

 [./vx]
 type = VelocityAux
 variable = v_x
 component = 0
 [../]

[]
 
[BCs]
 active = 'left_p right_p'
[./left_p]
type = DirichletBC
variable = pressure
boundary = '1'
value = 10e6
[../]	

[./left_t]
type = DirichletBC
variable = temperature
boundary = '1'
value = 100.
[../]

[./right_p]
type = DirichletBC
variable = pressure
boundary = '2'
value = 1e6
[../]

[./right_t]
    type = DirichletBC
    variable = temperature
    boundary = '2'
    value = 200.
[../]

[]

[Materials]
 active = 'rock'
 [./rock]
 type = Geothermal
 block = 1 


pressure = pressure
water_steam_properties = water_steam_properties

 gravity           =  0.0
 gx                =  0.0
 gy                =  0.0
 gz                =  1.0
 material_porosity    =  0.5
 permeability         =  1.0e-14
 density_rock         =  2500
 thermal_conductivity =  2.5E-6
 specific_heat_water  =	4186E-6
 specific_heat_rock   =  920E-6
 youngs_modulus       =   1.50e10
 poissons_ratio       =   0.3
 biot_coeff           =   1.0
 thermal_expansion    =   1.0e-6

 t_ref                =   200

 [../]
[]

[UserObjects]
active = 'water_steam_properties'

[./water_steam_properties]
type = WaterSteamEOS
[../]
[]


[Executioner]
 active = 'Quadrature'
 type =  Transient

 num_steps = 2
 dt = 10.0
 
 [./Quadrature]
  type = Trap
 [../]
 
[]
 
[Output]
 file_base = out
 output_initial = true
 interval = 1
 exodus = true
  print_out_info = true
[]
 
