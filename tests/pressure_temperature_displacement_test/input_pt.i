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
dim = 3
file = csiro_example_smaller_1.e
[ ]

[Variables]
 active = 'pressure temperature disp_x disp_y disp_z'
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

[./disp_x]
order = FIRST
family = LAGRANGE
#initial_from_file_var = 'disp_x'
#initial_from_file_timestep = 2
initial_condition = 0.0
#scaling = 1.0e-4
[../]

[./disp_y]
order = FIRST
family = LAGRANGE
#initial_from_file_var = 'disp_y'
#initial_from_file_timestep = 2
initial_condition = 0.0
#scaling = 1.0e-4
[../]

[./disp_z]
order = FIRST
family = LAGRANGE
#initial_from_file_var = 'disp_z'
#initial_from_file_timestep = 2
initial_condition = 0.0
#scaling = 1.0e-4
[../]

[ ]

#*********************************************
[Preconditioning]
# active = ' '
# active= 'PBP'
# active = 'FDP'
# active = 'SMP'	 
 active = 'SMP_newton'


[./PBP]
type= PBP
solve_order='pressure enthalpy'
preconditioner= 'LU LU'
off_diag_row= 'enthalpy'
off_diag_column= 'pressure'
[../]

[./FDP]
type = FDP
petsc_options =  '-snes_mf_operator -ksp_monitor'
petsc_options_iname = ' -pc_type -mat_fd_coloring_err -mat_fd_type'
petsc_options_value = ' lu 1.0e-9 ds'
#  off_diag_row    = 'pressure'
#  off_diag_column = 'enthalpy'
full = true
[../]

[./SMP]
type = SMP
petsc_options =  '-snes_mf_operator -ksp_monitor'
petsc_options_iname =  ' -pc_type -pc_hypre_type -ksp_gmres_restart'
petsc_options_value =  '  hypre boomeramg 201'
#     petsc_options_iname = ' -pc_type'
#     petsc_options_value = ' lu'
#    off_diag_row    = 'enthalpy'
#    off_diag_column = 'pressure'
#    petsc_options_iname =  ' -pc_type -sub_pc_type -ksp_gmres_restart'
#    petsc_options_value =  ' asm  ilu  201'
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


active = 'density_water viscosity_water
          v_x v_y tau_xx tau_yy tau_zz tau_xy tau_xz tau_yz
          s_xx s_yy s_zz s_xy s_xz s_yz'

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

[./tau_xx]
 order = CONSTANT
 family = MONOMIAL
[../]

[./tau_yy]
 order = CONSTANT
 family = MONOMIAL
[../]

[./tau_zz]
 order = CONSTANT
 family = MONOMIAL
[../]

[./tau_xy]
 order = CONSTANT
 family = MONOMIAL
[../]

[./tau_xz]
order = CONSTANT
family = MONOMIAL
[../]

[./tau_yz]
order = CONSTANT
family = MONOMIAL
[../]

[./s_xx]
order = CONSTANT
family = MONOMIAL
[../]

[./s_yy]
order = CONSTANT
family = MONOMIAL
[../]

[./s_zz]
order = CONSTANT
family = MONOMIAL
[../]

[./s_xy]
order = CONSTANT
family = MONOMIAL
[../]

[./s_xz]
order = CONSTANT
family = MONOMIAL
[../]

[./s_yz]
order = CONSTANT
family = MONOMIAL
[../]


[ ]

[Kernels]
 active = 'p_td p_wmfp t_td t_d t_c water_source heat_source
       dx_dt dy_dt dz_dt div_x div_y div_z poro_x poro_y poro_z temp_x temp_y temp_z'

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
value   = 3.2
point   = '0 0 0'
size    = '3.1 3.1 1.1' 
[../]

[./heat_source]
type = SourceSink
variable = temperature
value   = 1.3853302
point   = '0 0 0'
size    = '3.1 3.1 1.1' 
[../]

[./dx_dt]
type = SolidMechImplicitEuler
variable = disp_x
[../]

[./dy_dt]
type = SolidMechImplicitEuler
variable = disp_y
[../]

[./dz_dt]
type = SolidMechImplicitEuler
variable = disp_z
[../]

[./div_x]
type = SolidMechXFalcon
variable = disp_x
y = disp_y
z = disp_z
[../]

[./div_y]
type = SolidMechYFalcon
variable = disp_y
x = disp_x
z = disp_z
[../]

[./div_z]
type = SolidMechZFalcon
variable = disp_z
x = disp_x
y = disp_y
[../]

[./gravity_z]
type = Gravity
variable = disp_z
component = 2
[../]

[./poro_x]
type = SolidMechPoroCoupleX
variable = disp_x
pressure = pressure
[../]
[./poro_y]
type = SolidMechPoroCoupleY
variable = disp_y
pressure = pressure
[../]
[./poro_z]
type = SolidMechPoroCoupleZ
variable = disp_z
pressure = pressure
[../]

[./temp_x]
type = SolidMechTempCoupleXFalcon
variable = disp_x
temperature = temperature
[../]

[./temp_y]
type = SolidMechTempCoupleYFalcon
variable = disp_y
temperature = temperature
[../]

[./temp_z]
type = SolidMechTempCoupleZFalcon
variable = disp_z
temperature = temperature
[../]



[ ]

[AuxKernels]
 active = 'density_water viscosity_water vx vy
       comp_tau_xx comp_tau_yy comp_tau_zz comp_tau_xy comp_tau_xz comp_tau_yz 
       comp_s_xx comp_s_yy comp_s_zz comp_s_xy comp_s_xz comp_s_yz'

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

[./comp_tau_xx]
type = StressStrainDamageComputeAux
variable = tau_xx
quantity = 'stress'
component = 0
[../]
[./comp_tau_yy]
type = StressStrainDamageComputeAux
variable = tau_yy
quantity = 'stress'
component = 1
[../]
[./comp_tau_zz]
type = StressStrainDamageComputeAux
variable = tau_zz
quantity = 'stress'
component = 2
[../]
[./comp_tau_xy]
type = StressStrainDamageComputeAux
variable = tau_xy
quantity = 'stress'
component = 3
[../]
[./comp_tau_xz]
type = StressStrainDamageComputeAux
variable = tau_xz
quantity = 'stress'
component = 4
[../]
[./comp_tau_yz]
type = StressStrainDamageComputeAux
variable = tau_yz
quantity = 'stress'
component = 5
[../]

[./comp_s_xx]
type = StressStrainDamageComputeAux
variable = s_xx
quantity = 'strain'
component = 0
[../]
[./comp_s_yy]
type = StressStrainDamageComputeAux
variable = s_yy
quantity = 'strain'
component = 1
[../]
[./comp_s_zz]
type = StressStrainDamageComputeAux
variable = s_zz
quantity = 'strain'
component = 2
[../]
[./comp_s_xy]
type = StressStrainDamageComputeAux
variable = s_xy
quantity = 'strain'
component = 3
[../]
[./comp_s_xz]
type = StressStrainDamageComputeAux
variable = s_xz
quantity = 'strain'
component = 4
[../]
[./comp_s_yz]
type = StressStrainDamageComputeAux
variable = s_yz
quantity = 'strain'
component = 5
[../]


[ ]
 
[BCs]
  active = 'outside_p outside_t
            top_w bot_w side_confine_u1 side_confine_u2 side_confine_v1 side_confine_v2'
 [./outside_p]
    type = DirichletBC
    variable = pressure
    boundary = '3 4 5 6'
    value = 1.0e7
 [../]	  

 [./outside_t]
#    type = DirichletBC
    type = OutFlowBC
    variable = temperature
    boundary = '3 4 5 6'
#    value = 200.
 [../]

[./top_w]
type = NeumannBC
variable = disp_z
boundary = 1
value = -12.5e6
[../]

[./bot_w]
type = DirichletBC
variable = disp_z
boundary = 2
value = 0
[../]

[./side_confine_u1]
type = DirichletBC
variable = disp_x
boundary = 5
value = 0
[../]

[./side_confine_u2]
type = NeumannBC
variable = disp_x
boundary = 6
value = -25e6
[../]

[./side_confine_v1]
type = DirichletBC
variable = disp_y
boundary = 3
value =  0
[../]

[./side_confine_v2]
type = NeumannBC
variable = disp_y
boundary = 4
value = -25e6
[../]


[ ]

[Materials]
active = 'matrix frac'

[./frac]
type = Geothermal
block = 2 

x_disp = disp_x
y_disp = disp_y
z_disp = disp_z

pressure        = pressure
temperature     = temperature
water_steam_properties = water_steam_properties
temp_dependent = true

gravity           =  9.8
gx                =  0.0
gy                =  0.0
gz                =  0.0

material_porosity    =  0.1

permeability         =  1e-12

density_rock         =  2500

#look here....
thermal_conductivity =  2500E-6

specific_heat_water  =	4186E-6
specific_heat_rock   =  920E-6

youngs_modulus       =   1.50e10
poissons_ratio       =   0.3
biot_coeff           =   1.0

thermal_expansion    =   2.0e-5
t_ref                =   200

# has_crack = false
# has_damage = false
has_frac_perm = true
[../]

[./matrix]
type = Geothermal
block = 1 

x_disp = disp_x
y_disp = disp_y
z_disp = disp_z

pressure        = pressure
temperature     = temperature
water_steam_properties = water_steam_properties

gravity           =  9.8
gx                =  0.0
gy                =  0.0
gz                =  0.0

material_porosity    =  0.1

permeability         =  1e-16

density_rock         =  2500

#look here....
thermal_conductivity =  250E-6

specific_heat_water  =	4186E-6
specific_heat_rock   =  920E-6

youngs_modulus       =   1.50e10
poissons_ratio       =   0.3
biot_coeff           =   1.0

thermal_expansion    =   2.0e-6
t_ref                =   200

#has_crack = false
#has_damage = false
#has_frac_perm = false
[../]
[ ]

[UserObjects]
active = 'water_steam_properties'

[./water_steam_properties]
type = WaterSteamEOS
[../]
[ ]

[Executioner]
#  active = 'Adaptivity '
 active = 'Quadrature'
# active = ' '
# type = Steady
# type =  Transient

 type =  SolutionTimeAdaptive


 perf_log =  true
# petsc_options =  '-snes_mf_operator -ksp_monitor'
# petsc_options_iname =  '-pc_type -pc_hypre_type -ksp_gmres_restart'
# petsc_options_value =  'hypre boomeramg 100'

# petsc_options_iname =  ' -snes_ls -pc_type -pc_hypre_type -ksp_gmres_restart'
# petsc_options_value =  ' basic  hypre boomeramg 201' 

 l_max_its  =  60
# l_tol =  1.0e-6
 nl_max_its =  12
# nl_rel_tol =  1e-9
# nl_abs_tol = 1e-8
 num_steps = 3
 dt = 1.0
 dtmax= 864000.0
 dtmin= 1e-2
# end_time = 10000
 
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
# interval = 1
 exodus = true
 print_out_info = true
 [ ]
 
