# 3D matrix app doing thermo-hydro PorousFlow and receiving heat energy via a VectorPostprocessor from the 2D fracture App
# For details on this input file see the following MOOSE porous_flow documentation pages:
# https://mooseframework.inl.gov/modules/porous_flow/multiapp_fracture_flow_introduction.html
# https://mooseframework.inl.gov/modules/porous_flow/multiapp_fracture_flow_PorousFlow_3D.html

#this is creating the matrix volume with the origin at 0,0,0  The fracture is also at 0,0,0.
#The actual 3km depth will be accounted for by the pressure field using the function insitu_pp
[Mesh]
  uniform_refine = 0
  [generate]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 8
    xmin = -200
    xmax = 200
    ny = 40
    ymin = -1000
    ymax = 1000
    nz = 40
    zmin = -1000
    zmax = 1000
  []
[]

#The GlobalParams makes it so every object that needs a PorousFlowDictator will
# have it defined as dictator.  Without this, several materiasl and kernels
# would need to include "PorousFlowDictator = dictator"
[GlobalParams]
  PorousFlowDictator = dictator
[]

# Variables are the nonlinear degree of freedom being solved for at the nodes.
#  We are solving a 2 field porous flow simulation
[Variables]
  [matrix_P]
  []
  [matrix_T]
  []
[]

# These are initial conditions defined by functions.
[ICs]
  [matrix_P]
    type = FunctionIC
    variable = matrix_P
    function = insitu_pp
  []
  [matrix_T]
    type = FunctionIC
    variable = matrix_T
    function = insitu_T
  []
[]

#These are boundary conditions.  It helps the solve to have some dirichletBCs set for
# each variable, it helps "ground" the model.
[BCs]
  [./pore_pressure_top]
    type = DirichletBC
    variable = matrix_P
    boundary = 5
    value = 20
  [../]
  [./temperature_top]
    type = FunctionDirichletBC
    variable = matrix_T
    boundary = 5
    function = insitu_T
  [../]
  [./temperature_bot]
    type = FunctionDirichletBC
    variable = matrix_T
    boundary = 0
    function = insitu_T
  [../]
[]

# These are the temperatures and pressures functions.  The presssure is offset by
# the hydrostatic pressure.
[Functions]
  [insitu_pp]
    type = ParsedFunction
    value = '9.81*1000*(3000 - z)'
  []
  [insitu_T]
    type = ParsedFunction
    value = '363 - (0.025*z)'  #using cooling rate of 25K/km with T=90C at 3km
  []
[]


#This is the an "action" that sets up several kernels, materials, and Variables
# needed for a two field porous flow simulation.
[PorousFlowFullySaturated]
  coupling_type = ThermoHydro
  porepressure = matrix_P
  temperature = matrix_T
  fp = water
  gravity = '0 0 -9.81' # Note the value, because of pressure_unit
  pressure_unit = Pa
[]

# This defines the fluid equation of state.  Since this is for the matrix, we
# use a simple equation of state.
[FluidProperties]
  [water]
    type = SimpleFluidProperties # this is largely irrelevant here since we care about heat conduction only
    thermal_expansion = 0 # to prevent depressurization as the reservoir is cooled
  []
[]

#these are the matrix material properties.
[Materials]
  #Assume constant porosity.  https://mooseframework.inl.gov/source/materials/PorousFlowPorosityConst.html
  [porosity]
    type = PorousFlowPorosityConst
    porosity = 0.1 #this might be high
  []
  [permeability]
    type = PorousFlowPermeabilityConst
    permeability = '1E-16 0 0   0 1E-16 0   0 0 1E-16'
  []
  [internal_energy]
    type = PorousFlowMatrixInternalEnergy
    density = 2875
    specific_heat_capacity = 825
  []
  [aq_thermal_conductivity]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '2.83 0 0  0 2.83 0  0 0 2.83'
  []
[]

[Preconditioning]
#iterative preconditioner
  # [./original]
  #   type = SMP
  #   full = true
  #   petsc_options_iname = '-ksp_type -pc_type -sub_pc_type -sub_pc_factor_shift_type'
  #   petsc_options_value = 'gmres asm lu NONZERO'
  # [../]
  [./superlu]
    type = SMP
    full = true
    petsc_options_iname = '-ksp_type -pc_type -pc_factor_mat_solver_package'
    petsc_options_value = 'gmres lu superlu_dist'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  [TimeStepper]
    #this should take bigger timesteps as teh solution gets easier ie less nonlinear iterations
    type = IterationAdaptiveDT
    dt = 1
    growth_factor = 1.1
    optimal_iterations = 6
  []
  end_time = 31.54e6   #1 year in seconds
  line_search = 'none'
  automatic_scaling = true
  l_max_its = 60
  l_tol = 8e-3
  nl_forced_its = 1  #this makes sure it takes at least one nonlinear iteration
  nl_max_its = 40
  nl_rel_tol = 5e-05  # this is probably what it will converge on.
  nl_abs_tol = 1e-10 # since this uses automatic_scaling, it is important to set a small absolute tolerance
[]

#this is suppressing some output and
[Outputs]
  print_linear_residuals = false
  [matrix]
    type = Exodus
    sync_times = '1 10 100 200 300 400 500 600 700 800 900 1000 1100 1200 1300
    1400 1500 1600 1700 1800 1900 2000 2100 2200 2300 2400 2500 2600 2700 2800
    2900 3000 3100 3200 3300 3400 3500 3600 3700 3800 3900 4000 4100 4200 4300
    4400 4500 4600 4700 4800 4900 5000 5100 5200 5300 5400 5500 5600 5700 5800
    5900 6000 6100 6200 6300 6400 6500 6600 6700 6800 6900 7000 7100 7200 7300
    7400 7500 7600 7700 7800 7900 8000 8100 8200 8300 8400 8500 8600 8700 8800
    8900 9000 10000 11000 12000 13000 14000 15000 16000 17000 18000 19000 20000
    30000 50000 70000 100000 200000 300000 400000 500000 600000 700000 800000
    900000 1000000 1100000 1200000 1300000 1400000 1500000 1600000 1700000
    1800000 1900000 2000000 2100000 2200000 2300000 2400000 2500000 2600000
    2700000 2800000 2900000'
    sync_only = true
  []
[]


################################################################################

#  Everything below deals with the trasnfer and passing data to and from they
# subapp and applying the heat from the subapp.  Everything above is for a normal
# two field porous flow simulation without a discrete fracture.

################################################################################

#This applies the heat transfer assosciated with the fracture
# This adds a value directly into the nonlinear residual.
# This reads data from teh reporter named [heat_transfer_rate] which
# gets data transferred into it from fracture_app  (reporter is below)
[DiracKernels]
  [heat_from_fracture]
    type = ReporterPointSource
    variable = matrix_T
    value_name = heat_transfer_rate/transferred_joules_per_s
    x_coord_name = heat_transfer_rate/x
    y_coord_name = heat_transfer_rate/y
    z_coord_name = heat_transfer_rate/z
  []
[]
#this is the previously mentioned reporter used
[Reporters]
  [heat_transfer_rate]
    type = ConstantReporter
    real_vector_names = 'transferred_joules_per_s x y z'
    real_vector_values = '10000; 0; 0; 0'
    outputs = none
  []
[]

#These auxillary variables and their kerneles (next block) are used to compute the thermal conductivity
#between the fracture and matrix "skin"
#- the transfers are described here: https://mooseframework.inl.gov/modules/porous_flow/multiapp_fracture_flow_transfers.html
#- heat transfer through the skin is described here: https://mooseframework.inl.gov/modules/porous_flow/multiapp_fracture_flow_equations.html
#- The heat transfers and assumptions made in using these element normals and
#    lengths is described in detail here: https://mooseframework.inl.gov/modules/porous_flow/multiapp_fracture_flow_PorousFlow_3D.html
[AuxVariables]
  [normal_thermal_conductivity]
    family = MONOMIAL
    order = CONSTANT
  []
  [fracture_normal_x]
    family = MONOMIAL
    order = CONSTANT
    initial_condition = 0
  []
  [fracture_normal_y]
    family = MONOMIAL
    order = CONSTANT
    initial_condition = 1
  []
  [fracture_normal_z]
    family = MONOMIAL
    order = CONSTANT
    initial_condition = 0
  []
  [element_normal_length]
    family = MONOMIAL
    order = CONSTANT
  []
  [fracDensity_AMR]
  []
[]

[AuxKernels]
  #THIS IS VERY TUNABLE and controls the heat transfer between matrix and fracture
  # this isotropic, could be more complicated.
  [normal_thermal_conductivity_auxk]
    type = ConstantAux
    variable = normal_thermal_conductivity
    value = 5 # very simple in this case
  []
  [element_normal_length_auxk]
    type = PorousFlowElementLength
    variable = element_normal_length
    direction = 'fracture_normal_x fracture_normal_y fracture_normal_z'
  []
[]

#  This is the multiapp setup.
[MultiApps]
  [fracture_app]
    type = TransientMultiApp
    input_files = fracture_app.i
    #this is the default for execute_on.  It forces the subapp to be executed PorousFlowPointSourceFromPostprocessor
    #this could be set to timestep_end which would cause the subapp to execute after the main app (matrix)
    execute_on = TIMESTEP_BEGIN
    #sub_cycling=true allows multiple fracture steps per matrix step
    # this is good because the fracture requries a smaller timestep size than the matrix.
    #  this results in about 3-4 fracture steps per matrix step.
    sub_cycling = true  #this being true allows multiple fracture steps per matrix step
  []
[]

# This is the communication of data bewtween the matrix and fracture
#  The different types of transfers being used are described in:
#  https://mooseframework.inl.gov/modules/porous_flow/multiapp_fracture_flow_transfers.html
[Transfers]
  #the matrix elements recieve the fracture normals.  These are used in PorousFlowElementLength
  # to compute the element length which is transferred back to the subapp below.
  [normal_x_from_fracture]
    type = MultiAppNearestNodeTransfer
    direction = from_multiapp
    multi_app = fracture_app
    source_variable = normal_dirn_x
    variable = fracture_normal_x
  []
  [normal_y_from_fracture]
    type = MultiAppNearestNodeTransfer
    direction = from_multiapp
    multi_app = fracture_app
    source_variable = normal_dirn_y
    variable = fracture_normal_y
  []
  [normal_z_from_fracture]
    type = MultiAppNearestNodeTransfer
    direction = from_multiapp
    multi_app = fracture_app
    source_variable = normal_dirn_z
    variable = fracture_normal_z
  []

  # the matrix elements are transferring this data back to the fracture subapp
  # this data is used to compute the heat transfer coefficients between the matrix and fracture
  [element_normal_length_to_fracture]
    type = MultiAppNearestNodeTransfer
    direction = to_multiapp
    multi_app = fracture_app
    source_variable = element_normal_length
    variable = enclosing_element_normal_length
  []
  [element_normal_thermal_cond_to_fracture]
    type = MultiAppNearestNodeTransfer
    direction = to_multiapp
    multi_app = fracture_app
    source_variable = normal_thermal_conductivity
    variable = enclosing_element_normal_thermal_cond
  []

  # This transfer is used for mesh refinement.  This will show which matrix elements
  # contain a fracture.  The density will be greater than zero if they do.
  [from_sub_fracDensity_AMR]  #used for refinement.
    source_variable = density
    direction = from_multiapp
    variable = fracDensity_AMR
    type = MultiAppMeshFunctionTransfer
    multi_app = fracture_app
  []
  ########     ALL OF THE ABOVE TRANSFERS ARE FOR SETTING UP THE PROBLEM AND DO NOT CHANGE

  #######      THE BELOW TRANSFERS ARE COUPLING THE HEAT TRANSFER BETWEEN THE APPS
  # This is sending the temperature of the matrix elements to the fracture to compute
  # the heat transfer through the "skin" from the matrix to the fracture
  [T_to_fracture]
    type = MultiAppInterpolationTransfer
    direction = to_multiapp
    multi_app = fracture_app
    source_variable = matrix_T
    variable = transferred_matrix_T
  []
  #  This transfer is getting the heat transfer rate computed at every node on the fracture mesh
  #  These heat sources are applied with the ReporterPointSource
  [heat_from_fracture]
    type = MultiAppReporterTransfer
    direction = from_multiapp
    multi_app = fracture_app
    from_reporters = 'heat_transfer_rate/joules_per_s heat_transfer_rate/x heat_transfer_rate/y heat_transfer_rate/z'
    to_reporters = 'heat_transfer_rate/transferred_joules_per_s heat_transfer_rate/x heat_transfer_rate/y heat_transfer_rate/z'
  []
[]

# this refines the matrix mesh around the fracture.
[Adaptivity]
  marker = marker
  max_h_level = 2  #2 refinement levels.  For this mesh with h=50m, this results in a finest refinement level of 12.5m
  stop_time = 10  #refine over a couple of timesteps, needed to reach max refinement.  Refinement is expensive so you want it to stop.
  [./Markers]
    [./marker]
      type = ValueThresholdMarker
      variable = fracDensity_AMR
      refine = 0.1   #any matrix element with a fracture in it (i.e. fracDensity greater than zero) will be refined.
    [../]
  [../]
[]
