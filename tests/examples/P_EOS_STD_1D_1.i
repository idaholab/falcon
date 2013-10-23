# This is a simple single phase, isothermal, mass transport example problem on a 100mx10mx1m column.
# Pressure is initially 100 kPa. A BC of 110 kPa is applied to the left-hand side of the column, and
# is allowed diffused acrosss the domain until it reaches steady state. The standard GeothermalMaterial
# is used, with the water/steam EOS routine coupled in to determine variable density/viscosity
# This is the same problem as P_CONST_STD_1D_1.i, but with variable fluid properties

[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 100
  xmax = 100
  ymax = 10
[]

[Variables]
# Main pressure and temperature variables declared
# This is an isothermal problem (mass transfer only), but temperature is used for calculation of fluid props
# which are temp and pressure dependent

  [./pressure]
    initial_condition = 0.1e7 #<-- [Pa]
  [../]
  [./temperature]
    initial_condition = 293.15 #<-- [K]
  [../]
[]

[Kernels]
  [./p_wmfp]        # Mass diffusion kernel for PT (single-phase) problems
    type = WaterMassFluxPressure_PT
    variable = pressure
  [../]
  [./p_td]          # Pressure time derivative kernel for PT (single-phase) problems
    type = MassFluxTimeDerivative_PT
    variable = pressure
  [../]
  [./t_d]           #Heat diffusion kernel for PT (single-phase) problems
    type = TemperatureDiffusion
    variable = temperature
  [../]
[]


[BCs]
  [./left_p]
    type = DirichletBC
    variable = pressure
    boundary = left
    value = 0.2e7
  [../]
  [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = right
    value = 0.1e7
  [../]
  [./const_temp]
    type = DirichletBC
    variable = temperature
    boundary = 'right left'
    value = 293.15
  [../]
[]

[Materials]

  [./GeothermalMaterial]
    block = 0
    
    # flag booleans to define THMC problem
    heat_transport              = true      # T - flaged true to provide thermal_conductivity to temp diffusion kernel even though this is not a T problem
    fluid_flow                  = true      # H -
    solid_mechanics             = false     # M
    chemical_reactions          = false     # C
    
    # couple in main NL variables
    pressure                    = pressure
    temperature                 = temperature
        
    # material property inputs from PorousMedia (base class - parameters availible to all THMC materials)
    gravity                     = 0.0       # gravity magnitude [m/s^2]
    gx                          = 0.0       # x-component of gravity vector
    gy                          = 0.0       # y-component of gravity vector
    gz                          = 1.0       # z-component of gravity vector
    porosity                    = 0.2
    permeability                = 1.0e-12   # [m^2]
    #compressibility             = 1e-15
    
    # material property inputs from HeatTransport
    specific_heat_water         = 4186      # [J/(kg.K)]
    specific_heat_rock          = 920       # [J/(kg.K)]
    thermal_conductivity        = 2.5       # [W/(kg.K)]
    
    # material property inputs from FluidFlow (must have temp_dependent_fluid_props = true and temperature coupled in to get varaible density/viscosity
    temp_dependent_fluid_props  = true      # we want to have variable density and viscosity in this problem, so we use the water/steam EOS routine
    water_steam_properties      = water_steam_properties    # coupling of WaterSteamEOS UserObject below to use for calculation of fluid props        
                
  [../]
[]

[UserObjects]
  [./water_steam_properties]    # This user object contains the water and steam equations of state that are used to calculate variable fluid props
    type = WaterSteamEOS
  [../]
[]

[Executioner]
  type = Transient
  dt = 1
  num_steps = 5
  solve_type = PJFNK
  petsc_options_iname = 'pc_type -pc_hypre_type -ksp_gmres_restart -pc_hypre_boomeramg_strong_threshold'
  petsc_options_value = 'hypre boomeramg 201 0.7'
  nl_abs_tol = 1e-8
nl_rel_tol = 1e-6
[]

[Output]
  file_base = P_EOS_STD_1d_1_out
  output_initial = true
  interval = 1
  exodus = true
[]

