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
  [./pressure]
    initial_condition = 1e6   #<-- [Pa]
    scaling = 1e6
  [../]
  [./temperature]
    initial_condition = 473.15  #<-- [K]
  [../]
[]

[AuxVariables]
  [./permeability]          # permeability aux variable is required when running using the stochastic geothermal material. This aux variable provides the random permeability field to the material
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./velocity_x]            # This aux variables is not necessary to run the problem, it just display the velocity
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./velocity_y]            # This aux variables is not necessary to run the problem, it just display the velocity
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./viscosity_water]       # This aux variables is not necessary to run the problem, it just display the viscosity
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./density_water]         # This aux variables is not necessary to run the problem, it just display the density
    order = CONSTANT
    family = MONOMIAL
  [../]  
[]

[Kernels]
  [./mass_diffusion]        # Mass diffusion kernel for PT (single-phase) problems
    type = WaterMassFluxPressure_PT
    variable = pressure
  [../]
  [./mass_time]             # Pressure time derivative kernel for PT (single-phase) problems
    type = MassFluxTimeDerivative_PT
    variable = pressure
  [../]
  [./temp_diffusion]        # Heat diffusion kernel for PT (single-phase) problems
    type = TemperatureDiffusion
    variable = temperature
  [../]
  [./temp_convection]       # Heat convection kernel for PT (single-phase) problems
    type = TemperatureConvection
    variable = temperature
  [../]  
  [./temp_time]              # Heat time derivative kernel for PT (single-phase) problems
    type = TemperatureTimeDerivative
    variable = temperature
  [../]  
[]

[AuxKernels]
  [./permeability_field]    # permeability aux kernel is required when running using the stochastic geothermal material. This aux kernel provides the random permeability field to the material
    type = StochasticFieldAux
    variable = permeability
    file_name = IESE_Permeability.txt
  [../]
  [./velocity_x]            # This aux kernels below is not necessary to run the problem, it just display the velocity
    type = VelocityAux
    variable = velocity_x
    component = 0
  [../]
  [./velocity_y]            # This aux kernels below is not necessary to run the problem, it just display the velocity
    type = VelocityAux
    variable = velocity_y
    component = 0
  [../]  
  [./density_water]         # This aux kernels below is not necessary to run the problem, it just display the desnity
    type = MaterialRealAux
    variable = density_water
    property = density_water
  [../]
  [./viscosity_water]       # This aux kernels below is not necessary to run the problem, it just display the viscosity
    type = MaterialRealAux
    variable = viscosity_water
    property = viscosity_water
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
 active = 'Rock'
#the StochasticGeothermalMaterial action has a bug in how it assigns the permeability, using the old system for now

[./Rock]
    type = StochasticGeothermal
    block = 1
    pressure = pressure
    temperature = temperature
    temp_dependent_fluid_props = true
    permeability = permeability
    gravity = 0.0
    gx = 0.0
    gy = 0.0
    gz = 1.0
    water_steam_properties = water_steam_properties
  [../]

  [./StochasticGeothermalMaterial]
    block = 0
    
    # flag booleans to define THMC problem
    heat_transport              = true      # T
    fluid_flow                  = true      # H
    solid_mechanics             = false     # M
    chemical_reactions          = false     # C
    
    # couple in main NL variables
    pressure                    = pressure
    temperature                 = temperature
    
    # here is where we couple in the aux variable that stores the stochastic permeability field
    permeability = permeability
        
    # material property inputs from PorousMedia (base class - parameters availible to all THMC materials)
    gravity                     = 0.0       # gravity magnitude [m/s^2]
    gx                          = 0.0       # x-component of gravity vector
    gy                          = 0.0       # y-component of gravity vector
    gz                          = 1.0       # z-component of gravity vector
    porosity                    = 0.2
    
    # material property inputs from HeatTransport
    specific_heat_water         = 4186      # [J/(kg.K)]
    specific_heat_rock          = 920       # [J/(kg.K)]
    thermal_conductivity        = 2.5       # [W/(kg.K)]
    
    # material property inputs from FluidFlow (must have temp_dependent_fluid_props = true and temperature coupled in to get varaible density/viscosity)
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
  file_base = PT_EOS_STOCH_2d_1_out
  output_initial = true
  interval = 1
  exodus = true
  print_out_info = true
[]

