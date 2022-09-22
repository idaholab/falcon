# Cold water injection into one side of the fracture network, and production from the other side
injection_rate = 25 # kg/s

[Mesh]
  uniform_refine = 0  #this could be used to refine the entire mesh.  The starting mesh size is 10m.
  [single_frac]
    type = FileMeshGenerator  # reading mesh from file
    file = 'single_fracture.exo'  #mesh created in cubit from journal file single_fracture.jou
  []
  [injection_node]
      #dirichleteBC needs a nodeset to fix the temperature for the injection point.
      #The box is 10mx10m (~mesh size) in the xy plane which grabs 3 nodes (that is probably ok).
    type = BoundingBoxNodeSetGenerator
    input = single_frac
    bottom_left = '-5 370 -5'
    top_right = '5 380 5'
    new_boundary = injection_node
  []
[]

#The GlobalParams makes it so every object that needs a PorousFlowDictator will
# have it defined as dictator.  Without this, several materiasl and kernels
# would need to include "PorousFlowDictator = dictator" and gravity =....
[GlobalParams]
  PorousFlowDictator = dictator
  gravity = '0 0 -9.81'
[]

# Variables are the nonlinear degree of freedom being solved for at the nodes.
#  We are solving a 2 field porous flow simulation
[Variables]
  [frac_P]
  []
  [frac_T]
  []
[]

# These are initial conditions defined by functions.
[ICs]
  [frac_P]
    type = FunctionIC
    variable = frac_P
    function = insitu_pp
  []
  [frac_T]
    type = FunctionIC
    variable = frac_T
    function = insitu_T
  []
[]

#This is the an "action" that sets up several kernels, materials, and Variables
# needed for a two field porous flow simulation.
#  this is the same action used in the matrix.  This action is getting gravity
# from the GlobalParams block above.
[PorousFlowFullySaturated]
  coupling_type = ThermoHydro
  porepressure = frac_P
  temperature = frac_T
  fp = water
  pressure_unit = Pa
[]

# TEMPERATURE COUPLING IS APPLIED BY THIS KERNEL
#  - This kernel applies the heat to the fracture from the matrix-fracture temperature difference
#  - The heat applied to the fracture is then applied back onto the matrix to conserve energy.
#
# This is equation 4 in: https://mooseframework.inl.gov/modules/porous_flow/multiapp_fracture_flow_equations.html
# This is the kernel that converts the temperature difference between the transferred
# matrix temperature (transferred_matrix_T) and the fracture temperature (frac_T)
# This is also using the heat_transfer_coeffiecient computed by the heat_transfer_coefficient_auxk
# auxkernel which uses the matrix element normal lengths and matrix thermal
# conductivity passed down from the matrix app.
#
#This Kernel is also using save_in to save the residual contributions from this
#  to the auxVariable "joules_per_s" so that it can be read into the "heat_transfer_rate"
#  point value NodalValueSampler vectorPostprocessor and transferred to the main app to applied as a DiracKernel
#  This save_in ensures an energy balance between the matrix and fracture app.
#  This is described more in the Transfer of heat energy section of this page:
# https://mooseframework.inl.gov/modules/porous_flow/multiapp_fracture_flow_primer.html
[Kernels]
  [toMatrix]
    type = PorousFlowHeatMassTransfer
    variable = frac_T
    v = transferred_matrix_T
    transfer_coefficient = heat_transfer_coefficient
    save_in = joules_per_s
  []
[]

#The flow is more complicated in the fracture and so several auxvariables and auxkernels are needed
#to compute additional quantities for computation of the porosity and permeability.
#monomial are element quantities
#lagrange are nodal, and are usually set by default.s
[AuxVariables]
  #This is the matrix temperature that is being used to couple the matrix and fracture apps.
  # This is being transferred from the matrix and changes every matrix timestep
  # which is when transfers are initiated.  This requires an initial condition because the
  # fracture is executed prior to the matrix on the first timestep.
  [transferred_matrix_T]
    initial_condition = 363
  []

  # as mentioned above in the kernels block, this is computing the heat transfer coefficents from
  # the lenfth of the matrix elements containing the fractures
  [heat_transfer_coefficient]
    family = MONOMIAL
    order = CONSTANT
    #This needs an initial condition because on the first timestep
    #the fracture app is executed prior to the matrix app.  So on the first step, the fracture app
    #has no information from the matrix.  This results in no heat transfer from the matrix to fracture
    #on teh first step
    initial_condition = 0.0
  []
  [joules_per_s]
  []
  [aperture]
    family = MONOMIAL
    order = CONSTANT
  []
  [perm_times_app]
    family = MONOMIAL
    order = CONSTANT
  []
  [density]
    family = MONOMIAL
    order = CONSTANT
  []
  [viscosity]
    family = MONOMIAL
    order = CONSTANT
  []
  [insitu_pp]
  []

  ### The remaining Auxvariabls are holding transfer data used to intialize the simulation.
  # Following three are sending data to the matrix
  # holds data for transferring to the matrix app for determination of matrix element length
  [normal_dirn_x]
    family = MONOMIAL
    order = CONSTANT
  []
  [normal_dirn_y]
    family = MONOMIAL
    order = CONSTANT
  []
  [normal_dirn_z]
    family = MONOMIAL
    order = CONSTANT
  []

  #The following two are recieving data from the matrix
  #holds the data recieved from the matrix elements about the element normal length that will be used
  # to compute the heat_transfer_coefficient_auxk below
  [enclosing_element_normal_length]
    family = MONOMIAL
    order = CONSTANT
  []
  #recieved the matrix normal_thermal_conductivity from the transfer.
  [enclosing_element_normal_thermal_cond]
    family = MONOMIAL
    order = CONSTANT
  []
[]

# these auxkernels compute additional quantities used by material properties and kernels.
[AuxKernels]
  # these three normals are going to be passed to the matrix app to compute the matrix element length
  [normal_dirn_x_auxk]
    type = PorousFlowElementNormal
    variable = normal_dirn_x
    component = x
  []
  [normal_dirn_y]
    type = PorousFlowElementNormal
    variable = normal_dirn_y
    component = y
  []
  [normal_dirn_z]
    type = PorousFlowElementNormal
    variable = normal_dirn_z
    component = z
  []

  # This parsedAux will determine the element properties based on the equation, and other auxvariables
  # This is equation 7 on: https://mooseframework.inl.gov/modules/porous_flow/multiapp_fracture_flow_equations.html
  # the value for h_s given by the constant_expression is from equation 8 on the same page.
  [heat_transfer_coefficient_auxk]
    type = ParsedAux
    variable = heat_transfer_coefficient
    args = 'enclosing_element_normal_length enclosing_element_normal_thermal_cond'
    constant_names = h_s
    constant_expressions = 1E3 #This is the value being assigned to h_s.   Should be much bigger than thermal_conductivity / L ~ 1
    function = 'if(enclosing_element_normal_length = 0, 0, h_s * enclosing_element_normal_thermal_cond * 2 * enclosing_element_normal_length / (h_s * enclosing_element_normal_length * enclosing_element_normal_length + enclosing_element_normal_thermal_cond * 2 * enclosing_element_normal_length))'
  []

  #this is outputting the initial pressure field so that it can be used as Pref in PorousFlowPorosityLinear
  # it is only executed on initial because it doesn't change and everything needed to compute it is
  # available at initilization.
  [insitu_pp]
    type = FunctionAux
    execute_on = initial
    variable = insitu_pp
    function = insitu_pp
  []

  # the PorousFlowPropertyAux provides simplified access to material properties
  # by making them auxvariables.  These are being used for exodus output for display purposes only
  [aperture]
    type = PorousFlowPropertyAux
    variable = aperture
    property = porosity
  []
  [perm_times_app]
    type = PorousFlowPropertyAux
    variable = perm_times_app
    property = permeability
    row = 0
    column = 0
  []
  [density]
    type = PorousFlowPropertyAux
    variable = density
    property = density
    phase = 0
  []
  [viscosity]
    type = PorousFlowPropertyAux
    variable = viscosity
    property = viscosity
    phase = 0
  []
[]

#this BC is setting the temperature at the injection site (nodeset created by mesh generator)
#This needs to be the same location for the mass injection (inject_fluid)
[BCs]
  [inject_heat]
    type = DirichletBC
    boundary = injection_node
    variable = frac_T
    value = 313
  []
[]

[DiracKernels]
  #this describes teh injection of the fluid.  This is where the flow rate is defined.
  [inject_fluid]
    type = PorousFlowPointSourceFromPostprocessor
    mass_flux = 25 # kg/s
    point = '0 375 0'
    variable = frac_P
  []
  #the peaceman borehold is used to model production described at:
  #    https://mooseframework.inl.gov/source/dirackernels/PorousFlowPeacemanBorehole.html
  #The description for this case is described at:
  #    https://mooseframework.inl.gov/modules/porous_flow/multiapp_fracture_flow_PorousFlow_3D.html
  [withdraw_fluid]
    type = PorousFlowPeacemanBorehole
    SumQuantityUO = kg_out_uo #userObject below,
    bottom_p_or_t = 30.6e6 # 1MPa + approx insitu at production point, to prevent aperture closing due to low porepressures
    character = 1
    line_length = 1
    point_file = production_single_fracture.xyz
    unit_weight = '0 0 0'
    fluid_phase = 0
    use_mobility = true
    variable = frac_P
  []
  [withdraw_heat]
    type = PorousFlowPeacemanBorehole
    SumQuantityUO = J_out_uo
    bottom_p_or_t = 30.6e6 # 1MPa + approx insitu at production point, to prevent aperture closing due to low porepressures
    character = 1
    line_length = 1
    point_file = production_single_fracture.xyz
    unit_weight = '0 0 0'
    fluid_phase = 0
    use_mobility = true
    use_enthalpy = true
    variable = frac_T
  []
[]

#these userObjects produce output from teh PorousFlowPeacemanBorehole that is easy to plot
[UserObjects]
  [kg_out_uo]
    type = PorousFlowSumQuantity
  []
  [J_out_uo]
    type = PorousFlowSumQuantity
  []
[]

#these are the equations of state userObject used for the fluid.  A more complicated one is
#used in the fracture because of the pressure and temperature changes.
[FluidProperties]
  [true_water]
    type = Water97FluidProperties
  []
  [water]
    type = TabulatedFluidProperties
    fp = true_water
    temperature_min = 275 # K
    temperature_max = 600
    interpolated_properties = 'density viscosity enthalpy internal_energy'
    fluid_property_file = water97_tabulated.csv
  []
[]

[Materials]
  # This is described by equation 1 on this page:
  # https://mooseframework.inl.gov/modules/porous_flow/multiapp_fracture_flow_PorousFlow_3D.html
  [porosity]
    type = PorousFlowPorosityLinear
    porosity_ref = 1E-4 # fracture porosity = 1.0, but must include fracture aperture of 1E-4 at P = insitu_pp
    P_ref = insitu_pp
    P_coeff = 3e-10 # this is in metres/Pa, ie for P_ref = 1/P_coeff, the aperture becomes 1 metre
    porosity_min = 1E-5
  []
  [permeability]
    type = PorousFlowPermeabilityKozenyCarman
    k0 = 1E-15  # fracture perm = 1E-11 m^2, but must include fracture aperture of 1E-4
    poroperm_function = kozeny_carman_phi0
    m = 0
    n = 3
    phi0 = 1E-4
  []
  [internal_energy]
    type = PorousFlowMatrixInternalEnergy
    density = 2700 # kg/m^3
    specific_heat_capacity = 0 # basically no rock inside the fracture
  []
  [aq_thermal_conductivity]
    type = PorousFlowThermalConductivityIdeal
    # thermal conductivity of water times fracture aperture.
    # This should increase linearly with aperture, but is set constant in this model
    dry_thermal_conductivity = '0.6E-4 0 0  0 0.6E-4 0  0 0 0.6E-4'
  []
[]

[Functions]
  [kg_rate]
    type = ParsedFunction
    vals = 'dt kg_out'
    vars = 'dt kg_out'
    value = 'kg_out/dt'
  []
  [insitu_pp]
    type = ParsedFunction
    value = '9.81*1000*(3000 - z)'
  []
  [insitu_T]
    type = ParsedFunction
    value = '363 - (0.025*z)'  #using cooling rate of 25K/km with T=90C at 3km
  []
[]

# these are quantities of interest that will be output if csv=true in the [Outputs] block
#  the matrix_app.py file plots P_out
[Postprocessors]
  [dt]
    type = TimestepSize
    outputs = 'none'
  []
  [kg_out]
    type = PorousFlowPlotQuantity
    uo = kg_out_uo
  []
  [kg_per_s]
    type = FunctionValuePostprocessor
    function = kg_rate
  []
  [J_out]
    type = PorousFlowPlotQuantity
    uo = J_out_uo
  []
  [TK_out]
    type = PointValue
    variable = frac_T
    point = '0 -375 0'
  []
  [P_out]
    type = PointValue
    variable = frac_P
    point = '0 -375 0'
  []
  [P_in]
    type = PointValue
    variable = frac_P
    point = '0 375 0'
  []
[]

#  This vectorpostprocessor is being transferred to the matrix .
#  This creates a list of the nodal auxVariable joules_per_s so that it can be
#  transferred to the matrix app.  This is the best way to transfer lower dimensional data
#  from the fracture to a higher dimensional mesh of the matrix.
#  outputs=none suppresses output of this to a csv file.  It would create too much data.
[VectorPostprocessors]
  [heat_transfer_rate]
    type = NodalValueSampler
    outputs = none
    sort_by = id
    variable = joules_per_s
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
  #  direct solver preconditioner (exact but a little expensive for big problems)
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
    #this should take bigger timesteps as the solution gets easier ie less nonlinear iterations
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

#this controls the outputs. This is using the same sync_times for the exodus output
# used on the matrix app.  This makes it nicer to visualize them together
# csv output is also true for all the post processors.
[Outputs]
  print_linear_residuals = false
  csv = true
  [frac]
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
