# This is a single phase, non-isothermal, advection-diffusion example problem on a 63mx63m square.
# Pressure is initially 1 MPa and temperature 200 C. BCs of 10 MPa and 100 C are applied to the 
# left-hand side of the domain, and are allowed propigate acrosss.  The StochasticGeothermalMaterial
# is used to provide a stochastic permeability field and the water/steam EOS routine is coupled in to 
# determine pressure/temperature dependent density and viscosity

#19 Oct 2013 - the material properties need to be updated.  There is a bug in the StochasticGeothermalMaterial Action 
#that tried to double allocate the permeability. This example is currently using the "old" material setup
#with diamond inheritance.  It needs to be updated.

[Mesh]
  type = FileMesh
  file = 3d_stochastic_course.e
[]

[Variables]
  # Main pressure and temperature variables declared
  [./pressure]
    initial_condition = 1e6 # <-- [Pa]
    scaling = 1e6
  [../]
  [./temperature]
    initial_condition = 473.15 # <-- [K]
  [../]
[]

[AuxVariables]
  # permeability aux variable is required when running using the stochastic geothermal material.
  # this aux variable provides the random permeability field to the material
  # The aux variables below are not necessary to run the problem, they just display the velocity/density/viscosity, which change due to
  # pressure effects
  [./permeability]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./velocity_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./velocity_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./viscosity_water]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./density_water]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./tau]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./mass_diffusion]
    # Mass diffusion kernel for PT (single-phase) problems
    type = WaterMassFluxPressure_PT
    variable = pressure
  [../]
  [./mass_time]
    # Pressure time derivative kernel for PT (single-phase) problems
    type = MassFluxTimeDerivative_PT
    variable = pressure
  [../]
  [./temp_diffusion]
    # Heat diffusion kernel for PT (single-phase) problems
    type = TemperatureDiffusion
    variable = temperature
  [../]
  [./temp_convection]
    # Heat convection kernel for PT (single-phase) problems
    type = TemperatureConvection
    variable = temperature
  [../]
  [./temp_time]
    # Heat time derivative kernel for PT (single-phase) problems
    type = TemperatureTimeDerivative
    variable = temperature
  [../]
[]

[AuxKernels]
  # permeability aux kernel is required when running using the stochastic geothermal material.
  # this aux kernel provides the random permeability field to the material
  # These aux kernels below are not necessary to run the problem, they just display the velocity/density/viscosity, which change due to
  # pressure effects
  [./permeability_field]
    type = StochasticFieldAux
    variable = permeability
    file_name = IESE_Permeability.txt
  [../]
  [./velocity_x]
    type = DarcyFluxAux
    variable = velocity_x
  [../]
  [./velocity_y]
    type = DarcyFluxAux
    variable = velocity_y
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
  [./tau]
    type = MaterialRealAux
    variable = tau
    property = tau_water
  [../]
[]

[BCs]
  [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = 1
    value = 10e6
  [../]
  [./right_t]
    type = DirichletBC
    variable = temperature
    boundary = 1
    value = 373.15
  [../]
  [./left_p]
    type = DirichletBC
    variable = pressure
    boundary = 2
    value = 1e6
  [../]
  [./left_t]
    type = DirichletBC
    variable = temperature
    boundary = 2
    value = 473.15
  [../]
[]

[Materials]
  active = 'rock'
#the StochasticGeothermalMaterial action has a bug in how it assigns the permeability, using the old system for now

[./rock]
    type = StochasticGeothermal
    block = 1
    pressure = pressure
    temperature = temperature
    temp_dependent_fluid_props = false
    permeability = permeability
    gravity = 0.0
    gx = 0.0
    gy = 0.0
    gz = 1.0
  [../]

  [./StochasticGeothermalMaterial]
    # flag booleans to define THMC problem
    # couple in main NL variables
    # here is where we couple in the aux variable that stores the stochastic permeability field
    # material property inputs from PorousMedia (base class - parameters availible to all THMC materials)
    # material property inputs from HeatTransport
    # material property inputs from FluidFlow (must have temp_dependent_fluid_props = true and temperature coupled in to get varaible density/viscosity)
    block = 0
    heat_transport = true # T
    fluid_flow = true # H
    solid_mechanics = false # M
    chemical_reactions = false # C
    pressure = pressure
    temperature = temperature
    permeability = permeability
    gravity = 0.0 # gravity magnitude [m/s^2]
    gx = 0.0 # x-component of gravity vector
    gy = 0.0 # y-component of gravity vector
    gz = 1.0 # z-component of gravity vector
    porosity = 0.2
    temp_dependent_fluid_props = false # we want to have variable density and viscosity in this problem, so we use the water/steam EOS routine
   # water_steam_properties = water_steam_properties # coupling of WaterSteamEOS UserObject below to use for calculation of fluid props        
  [../]
[]

#[UserObjects]
  # This user object contains the water and steam equations of state that are used to calculate variable fluid props
#  [./water_steam_properties]
 #   type = WaterSteamEOS
#  [../]
#[]

[Executioner]
 type = Transient
  num_steps = 10
  dt = 2500000.0
  nl_abs_tol = 1e-6
  solve_type = PJFNK
  line_search = basic

  [./Quadrature]
    type = Trap
  [../]
[]


[Output]
  file_base = PT_CONST_STOCH_2d_1_out
  output_initial = true
  interval = 1
  exodus = true
  print_out_info = true
[]

