################################################################################################################
# A simple themo-poro-elastic example for iceland injection
# injection of 50 degree water into 500x500x500 m^3 reservoir at rate of 24Kg/s (using Trap integration rule)
# initial reservoir pressure & temperature are 10MP and 200, respectively
# initial vetitial stress is set to be 25MP (~1km over burden), and initial horizontal confining stress is 12.5MP
#
# !! a relatively large linear tolerance is used since mechanical deformation reach equilibrium very quickly
# !! the pressure also reach near steady state fairly quicky
# !! cooling zone also reaches quasi-steady at later stage: temperature change very slowing
################################################################################################################
[Mesh]
  dim = 2
  file = 2d_adaptive_4.e
[ ]

[Variables]
  active = 'pressure temperature'
  [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_condition = 1.0e7
  [../]

  [./temperature]
   order = FIRST
   family = LAGRANGE
   initial_condition = 200.0
  [../]
[ ]


[Preconditioning]
# active = 'FDP'
 active = 'SMP_newton'

[./FDP]
type = FDP
petsc_options =  '-snes_mf_operator -ksp_monitor'
petsc_options_iname = ' -pc_type -mat_fd_coloring_err -mat_fd_type'
petsc_options_value = ' lu 1.0e-9 ds'
#  off_diag_row    = 'pressure'
#  off_diag_column = 'enthalpy'
full = true
[../]

[./SMP_newton]
type = SMP
petsc_options =  '-snes -ksp_monitor'
#petsc_options =  '-snes '
petsc_options_iname =  ' -pc_type -pc_hypre_type -ksp_gmres_restart -snes_ls'
petsc_options_value =  '  hypre boomeramg 201 basic'
#     petsc_options_iname =  ' -pc_type -sub_pc_type -ksp_gmres_restart'
#     petsc_options_value =  ' asm  ilu  201'
#     petsc_options_iname = ' -pc_type'
#     petsc_options_value = ' lu'
#    off_diag_row    = 'enthalpy'
#    off_diag_column = 'pressure'
full = true
[../]

[ ]


[AuxVariables]
# active = 'density_water viscosity_water v_x v_y'
 active = 'density_water viscosity_water'

[./density_water]
order = CONSTANT
family = MONOMIAL
[../]

[./viscosity_water]
order = CONSTANT
family = MONOMIAL
[../]

[./v_x]
order = CONSTANT
family = MONOMIAL
[../]

[./v_y]
order = CONSTANT
family = MONOMIAL
[../]
[ ]

[Kernels]
 active = 'p_td p_wmfp t_td t_d t_c water_source heat_source'
 
 [./p_td]
 type = MassFluxTimeDerivative_PT
 variable = pressure
 [../]

 [./p_wmfp]
 type = WaterMassFluxPressure_PT
 variable = pressure
 [../]
 
 [./t_td]
 type = TemperatureTimeDerivative
 variable = temperature
 [../]

 [./t_d]
 type = TemperatureDiffusion
 variable = temperature 
 [../]
 
 [./t_c]
 type = TemperatureConvection
 variable = temperature
 [../]

 [./water_source]
 type = SourceSink
 variable = pressure
 value   = 0.1
 point   = '50.0 25.0'
 size    = '1.01 3.01' 
 [../]

 [./heat_source]
 type = SourceSink
 variable = temperature
 value   = 0.1352078
 point   = '50.0 25.0'
 size    = '1.01 3.01' 
 [../]
[ ]

[AuxKernels]
# active = 'density_water viscosity_water vx vy'
 active = 'density_water viscosity_water'

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
[ ]
 
[BCs]
  active = 'outside_p outside_t'
 [./outside_p]
    type = DirichletBC
    variable = pressure
    boundary = '3 4'
    value = 1.0e7
 [../]	  

 [./outside_t]
#    type = DirichletBC
    type = OutFlowBC
    variable = temperature
    boundary = '3 4'
#    value = 200.
 [../]
[ ]

[Materials]
 active = 'rock fracture'
 [./rock]
 type = Geothermal
 block = 1 


  pressure        = pressure
  temperature     = temperature
  water_steam_properties = water_steam_properties
  temp_dependent = true

 gravity           =  0.0
 gx                =  0.0
 gy                =  0.0
 gz                =  1.0
 material_porosity    =  0.65
 permeability         =  1.0e-15
 density_rock         =  2500
 thermal_conductivity =  7.5E-5
 specific_heat_water  =	4186E-6
 specific_heat_rock   =  920E-6
 youngs_modulus       =   1.50e10
 poissons_ratio       =   0.3
 biot_coeff           =   1.0
 thermal_expansion    =   1.0e-6

 t_ref                =   200

 has_crack = false
 has_damage = false
 [../]

 [./fracture]
 type = Geothermal
 block = 2 

  pressure        = pressure
  temperature     = temperature
  water_steam_properties = water_steam_properties
  temp_dependent = true

 gravity           =  0.0
 gx                =  0.0
 gy                =  0.0
 gz                =  1.0
 material_porosity    =  0.65
 permeability         =  1.0e-13
 density_rock         =  2500
 thermal_conductivity =  7.5E-5
 specific_heat_water  =	4186E-6
 specific_heat_rock   =  920E-6
 youngs_modulus       =   1.50e10
 poissons_ratio       =   0.3
 biot_coeff           =   1.0
 thermal_expansion    =   1.0e-6

 t_ref                =   200

 has_crack = false
 has_damage = false
 [../]

[ ]


[UserObjects]
active = 'water_steam_properties'

[./water_steam_properties]
type = WaterSteamEOS
[../]
[ ]

 
[Executioner]
  active = 'Adaptivity '
# active = 'Quadrature'
# active = ' '
# type = Steady
# type =  Transient
 type =  SolutionTimeAdaptive
 perf_log =  true
 petsc_options =  '-snes_mf_operator -ksp_monitor'
 petsc_options_iname =  '-pc_type -pc_hypre_type -ksp_gmres_restart'
 petsc_options_value =  'hypre boomeramg 100'
# petsc_options_iname =  ' -snes_ls -pc_type -pc_hypre_type -ksp_gmres_restart'
# petsc_options_value =  ' basic  hypre boomeramg 201' 
 l_max_its  =  100
# l_tol =  1.0e-6
 nl_max_its =  12
 nl_rel_tol =  1e-9
 nl_abs_tol = 1e-13
 num_steps = 5 
 dt = 10.0
 dtmax= 864000.0
 dtmin= 1e-2
# end_time = 8640000
 
 [./Adaptivity]
 initial_adaptivity = 1
 error_estimator = KellyErrorEstimator
 refine_fraction  = 0.9
 coarsen_fraction = 0.001
 max_h_level = 2
 [../]
 
 [./Quadrature]
  type = Trap
 [../]
 
[ ]
 
[Output]
 file_base = out
 output_initial = true
 interval = 1
 exodus = true
#  print_out_info = true
 [ ]
 
