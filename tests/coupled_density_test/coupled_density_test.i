[Mesh]
#  dim = 3
  file = 1d_col.e
# uniform_refine = 2

[ ]

[Variables]
#   active = 'pressure temperature x_disp y_disp z_disp'
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

  [./x_disp]
    order = FIRST
    family = LAGRANGE
  [../]

  [./y_disp]
    order = FIRST
    family = LAGRANGE
  [../]

  [./z_disp]
    order = FIRST
    family = LAGRANGE
  [../]

[ ]
 
 
[AuxVariables]
 active = ' dwdt dwdp density_water viscosity_water'
 
[./density_water]
 order = FIRST
 family = LAGRANGE
 initial_condition = 871.06
 [../]

[./dwdt]
 order = FIRST
 family = LAGRANGE
 initial_condition = 0.0
 [../]


[./dwdp]
 order = FIRST
 family = LAGRANGE
 initial_condition = 0.0
 [../]
 
 
[./viscosity_water]
 order = FIRST
 family = LAGRANGE
 initial_condition = 1.34E-4
 [../]
 
#this is for material version
 [./porosity]
 order = FIRST
 family = LAGRANGE
 initial_condition = 0.3
 [../]
[ ]

[Kernels]
#active = ' p_wmfp t_d'
#active = 'p_wmfp t_d'
#active = 'p_td  p_wmfp t_td'
active = 'p_td t_td  t_d t_c'
 
[./p_td]
 type = MassFluxTimeDerivative
 variable = pressure
 density_water = density_water
 dwdp          = dwdp
 [../]

 [./p_wmfp]
 type = WaterMassFluxPressure
# density_water = density_water
# viscosity_water=viscosity_water
 variable = pressure
 [../]
 
 [./p_wmfe]
 type = WaterMassFluxElevation
 density_water = density_water
 variable = pressure
 [../]
 
 [./t_td]
 type = TemperatureTimeDerivative
 variable = temperature
 density_water = density_water
 dwdt          = dwdt
#  porosity = porosity
 [../]

 [./t_d]
 type = TemperatureDiffusion
 variable = temperature 
 [../]
 
 [./t_c]
 type = TemperatureConvection
 variable = temperature
 [../]

[ ]

 [AuxKernels]
 active = 'density_water viscosity_water dwdt dwdp'

[./density_water]
 type = CoupledDensityAux
 variable = density_water
 temperature = temperature
 pressure = pressure
 density_water =871.06
 dwdt=dwdt
 dwdp=dwdp
 temp_dependent_density = true
[../]

[./dwdt]
 type = CoupledDdensityDTAux
 variable = dwdt
 temperature = temperature
 pressure = pressure
 dwdt=1.0
[../]
 
 [./dwdp]
 type = CoupledDdensityDPAux
 variable = dwdp
 temperature = temperature
 pressure = pressure
 dwdp=-1.0
[../]
 
 
[./viscosity_water]
 type = CoupledViscosityAux
 variable = viscosity_water
 temperature = temperature
 pressure = pressure
 viscosity_water = 1.34E-06
 temp_dependent_viscosity = true
[../]
 
 
[ ]
 
 
 
[BCs]
   active = 'left_p right_p left_t right_t'

  [./left_p]
    type = DirichletBC
    variable = pressure
    boundary = 1
    value = 1.05e7
  [../]

   [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = 2
    value = 1.0e7
  [../]

  [./left_t]
    type = DirichletBC
    variable = temperature
    boundary = 1
    value = 50.0
  [../]

  [./right_t]
 type = DirichletBC
 variable = temperature
 boundary = 2
 value = 200.0
 [../]
 
[ ]

[Materials]

 active = 'rock'
 
 [./rock]
 type = Geothermal
 block = 1 
 
 pressure = pressure
 density_water = density_water
 viscosity_water = viscosity_water
 temperature = temperature

 material_porosity = 0.2
 
 gravity              =  0.0
 gx = 0
 gy = 0
 gz = 1
 permeability         =  1.0e-15
 density_rock         =  2500
 thermal_conductivity =  2.5E-4
 specific_heat_water  =	4186E-4
 specific_heat_rock   =  920E-4

 has_solid_mechanics   = false
 
 
[../]

 [./frac]
 type = Geothermal
 block = 2
 
 pressure = pressure
 density_water = density_water
 viscosity_water = viscosity_water
 temperature = temperature
 
 material_porosity = 0.60
 
 gravity              =  0.0
 gx = 0
 gy = 0
 gz = 1
 permeability         =  1.0e-11
 density_rock         =  2500
 thermal_conductivity =  2.5E-3
 specific_heat_water  =	4186E-3
 specific_heat_rock   =  920E-3
 
 has_solid_mechanics   = false
 
 
[../]
 
[ ]
 
[Executioner]
#active = 'Adaptivity '
 active = 'Quadrature'

# type = Steady
 type =  Transient
# type =  SolutionTimeAdaptive
 perf_log =  true
 
 petsc_options =  '-snes_mf_operator -ksp_monitor'
# petsc_options_iname =  '-pc_type -sub_pc_type'
# petsc_options_value = 'asm ilu'
 petsc_options_iname =  '-pc_type -pc_hypre_type -ksp_gmres_restart'
 petsc_options_value =  'hypre boomeramg 501'
 
 l_max_its  =  500
 nl_abs_tol = 1e-12
 l_tol =  1.0e-6
 nl_max_its =  60
# nl_rel_tol =  1e-6
 
 
 num_steps = 10 
 
 
 dt = 100.
# dtmax=200.0
# dtmin= 1E-2
# dtmax = 8640
# end_time = 7776000
 
 
 [./Adaptivity]
 initial_adaptivity = 1
 error_estimator = KellyErrorEstimator
 refine_fraction  = 0.9
 coarsen_fraction = 0.001
#    weight_names = 'temperature'
#   weight_values = 0.8
 max_h_level = 2
# steps = 2
 [../]
 
 [./Quadrature]
  type = Trap
[../]

 
 [ ]
 
[Output]
 file_base = out
 output_initial = true
 interval = 20
 exodus = true

#  print_out_info = true
 [ ]
 
