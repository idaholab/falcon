[Mesh]
#  dim = 3
  file = 1d_col.e
# uniform_refine = 2

[ ]

[Variables]
#   active = 'pressure temperature x_disp y_disp z_disp'
  active = 'pressure enthalpy'
   [./pressure]
    order = FIRST
    family = LAGRANGE
    initial_condition = 5e6
   [../]

   [./enthalpy]
   order = FIRST
   family = LAGRANGE
   initial_condition = 2850E-3
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
 active = 'density density_water density_steam enthalpy_water enthalpy_steam
           ddensitydH_P ddensitydp_H denthalpy_steamdH_P denthalpy_waterdH_P
           saturation_water dswdH dTdH_P temperature 
           viscosity_water viscosity_steam water_velocity
           denthalpy_waterdP_H denthalpy_steamdP_H dTdP_H'
#           ddensity_waterdP_H ddensity_steamdP_H
           

[./density]
 order = FIRST
 family = LAGRANGE
initial_condition = 878.98804480291119
[../]

[./density_water]
 order = FIRST
 family = LAGRANGE
 initial_condition = 0
 [../]

 
[./density_steam]
 order = FIRST
 family = LAGRANGE
 initial_condition = 878.98804480291119
 [../]

[./viscosity_water]
 order = FIRST
 family = LAGRANGE
 initial_condition = 1.81312661535894764E-004
 [../]

[./viscosity_steam]
 order = FIRST
 family = LAGRANGE
 initial_condition = 1.34E-5
 [../]

[./enthalpy_water]
 order = FIRST
 family = LAGRANGE
 initial_condition = 2.80
[../]

[./enthalpy_steam]
 order = FIRST
 family = LAGRANGE
 #initial_condition = 878.98804480291119
 initial_condition = 2.8
[../]

[./saturation_water]
 order = FIRST
 family = LAGRANGE
 initial_condition = 1.
[../]

[./temperature]
 order = FIRST
 family = LAGRANGE
 [./InitialCondition]
      type = LinearDisEnthalpyIC
      basevalue = 192
      slope = -0.027210884353741496598
     [../]
# initial_condition = 188.0  
[../]

#  ------------   end of properties -------

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
#+++++++++++++++++++++++++++++++

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
#++++++++++++++++++++++++++++++++++++
# ------------------------------------------------------
[./dswdH]
 order = FIRST
 family = LAGRANGE
[../]
 
 
[./dTdH_P]
 order = FIRST
 family = LAGRANGE
[../]

[./water_velocity]
 order=CONSTANT
 family=MONOMIAL
[../]
 
# ------------------------------------------------------
#this is for material version
 [./porosity]
 order = FIRST
 family = LAGRANGE
 initial_condition = 0.3
 [../]
[ ]

 # ***************************************************** 


[Kernels]
 active = 'p_td p_wmfp p_wsfp t_td  t_d t_cw t_cs'

# -----mass conservation kernels --------------------- 
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

 
 [./p_wmfe]
 type = WaterMassFluxElevation
 density_water = density_water
 variable = pressure
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
# **********************************************************

 [AuxKernels]
 active = 'density density_water density_steam enthalpy_water enthalpy_steam
           ddensitydH_P ddensitydp_H denthalpy_steamdH_P denthalpy_waterdH_P
           saturation_water dswdH dTdH_P temperature 
           viscosity_water viscosity_steam water_velocity
           denthalpy_waterdP_H denthalpy_steamdP_H dTdP_H'
#           ddensity_waterdP_H ddensity_steamdP_H
        
           

# --------------------------------------------------------
[./water_velocity]
 type = VelocityAux
 variable = water_velocity
 component = 0
[../]

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
#+++++++++++++++++++++++++++++++++++ 
#[./ddensity_waterdP_H]
#  type = CoupledDwaterdensitydP_HAux
#  variable = ddensity_waterdP_H
# [../]

# [./ddensity_steamdP_H]
#  type =  CoupledDsteamdensitydP_HAux
#  variable = ddensity_steamdP_H
# [../]


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

#+++++++++++++++++++++++++++++++++++
 
# ------------------------------------------------------

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

 
[ ]
 
# ************************************************************* 
 
[BCs]
#   active = 'left_p right_p left_t right_t'
  active = 'left_t left_p right_p right_t'

  [./left_p]
  #  type = DirichletBC
    type = NeumannBC 
    variable = pressure
    boundary = 1
 #   value = 5.01e6
    value = 881.06645536150575e-7
  [../]

   [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = 2
    value = 5e6
  [../]

  [./left_t]
  #  type = DirichletBC
    type = NeumannBC 
    variable = enthalpy
    boundary = 1
#    initial =1200E-3
#    final= 700E-3
#    durition = 2000
#    value = 1000E-03
    value =352.4266E-7
  [../]

 [./right_t]
   type = DirichletBC
   variable = enthalpy
   boundary = 2
   value = 2850E-3
 [../]
 
[ ]

[Materials]

 active = 'rock '
 
 [./rock]
 type = Geothermal
 block = 1 
 
 pressure = pressure
 enthalpy = enthalpy
 temperature = temperature
 density_water = density_water
 viscosity_water = viscosity_water
  
 material_porosity = 0.2
 
 gravity              =  0.0
 gx = 0
 gy = 0
 gz = 1
 permeability         =  1.0e-12
 density_rock         =  2500
 thermal_conductivity =  2.5E-6
 specific_heat_water  =	4186E-6
 specific_heat_rock   =  920E-6

 has_solid_mechanics   = false
 
 
[../]

 
[ ]

 
[Executioner]
#active = 'Adaptivity '
 active = 'Quadrature'
#active = ' '

# type = Steady
# type =  Transient
 type =  SolutionTimeAdaptive
 perf_log =  true
 
 petsc_options =  '-snes_mf_operator -ksp_monitor'
# petsc_options_iname =  '-snes_ls'
# petsc_options_value = 'basic'

# petsc_options_iname =  '-pc_type -sub_pc_type'
# petsc_options_value = 'ilu ilu'
 petsc_options_iname =  ' -snes_ls -pc_type -pc_hypre_type -ksp_gmres_restart'
 petsc_options_value =  ' basic  hypre boomeramg 201'
# petsc_options_iname =  ' -pc_type  -ksp_gmres_restart'
# petsc_options_value =  '  ilu    101'

 
 l_max_its  =  200
 nl_abs_tol = 1e-10
# l_tol =  1.0e-7
 nl_max_its =  60
 nl_rel_tol =  1e-6
 
 
 
 num_steps = 5 
 
 
 dt = 1.
 dtmax=2000.0
 dtmin= 1E-2
# dtmax = 8640
 end_time = 8640000
 
 
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
 interval = 1
 exodus = true

#  print_out_info = true
 [ ]
 
