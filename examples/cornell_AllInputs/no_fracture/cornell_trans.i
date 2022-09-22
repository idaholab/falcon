#Example for Cornell diect use system
#R Podgorney
#2021.08.24

#part 2--transient simulation


[Mesh]
  file = cornell_SS_out.e
[]

#############################################################
[Adaptivity]
    steps = 3
    marker = box
    [./Markers]
      [./box]
        bottom_left = '-200 1000 -2000'
        inside = refine
        top_right = '200 800 -2200'
        outside = do_nothing
        type = BoxMarker
      [../]
    [../]
[]



#############################################################
[GlobalParams]
  PorousFlowDictator = dictator
[]
############################################################
[Variables]
  [./P]
    initial_from_file_var = P
    initial_from_file_timestep = 'LATEST'
  [../]

  [./T]
    initial_from_file_var = T
    initial_from_file_timestep = 'LATEST'
    scaling = 1e-8
  [../]
[]
###########################################################
[BCs]
  [./pore_pressure_top]
    type = DirichletBC
    variable = P
    boundary = 5
    value = 20e6
  [../]

  [./temperature_top]
    type = DirichletBC
    variable = T
    boundary = 5
    value = 338.15
  [../]

  [./temperature_bot]
    type = DirichletBC
    variable = T
    boundary = 0
    value = 388.15
  [../]
[]
###########################################################
[AuxVariables]
  [./vel_x]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./vel_y]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./vel_z]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./density]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./viscosity]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]
###########################################################
[Kernels]
  [./P_time_deriv]
    type = PorousFlowMassTimeDerivative
    fluid_component = 0
    variable = P
  [../]

  [./energy_dot]
    type = PorousFlowEnergyTimeDerivative
    variable = T
  [../]

  [./P_flux]
    type = PorousFlowAdvectiveFlux
    fluid_component = 0
    variable = P
    gravity = '0 0 -9.8'
  [../]

  [./heat_conduction]
    type = PorousFlowHeatConduction
    variable = T
  [../]

  [./heat_advection]
    type = PorousFlowHeatAdvection
    variable = T
    gravity = '0 0 -9.8'
  [../]
[]
##############################################################
[AuxKernels]
  [./vel_x]
    type = PorousFlowDarcyVelocityComponent
    variable = vel_x
    component = x
    fluid_phase = 0
    gravity = '0 0 -9.8'
  [../]

  [./vel_y]
    type = PorousFlowDarcyVelocityComponent
    variable = vel_y
    component = y
    fluid_phase = 0
    gravity = '0 0 -9.8'
  [../]

  [./vel_z]
    type = PorousFlowDarcyVelocityComponent
    variable = vel_z
    component = z
    fluid_phase = 0
    gravity = '0 0 -9.8'
  [../]

  [./density]
    type = MaterialRealAux
     variable = density
     property = density
  [../]

  [./viscosity]
    type = MaterialRealAux
     variable = viscosity
     property = viscosity
  [../]
[]
############################################################
[UserObjects]
  [./dictator]
    type = PorousFlowDictator
    porous_flow_vars = 'P T'
    number_fluid_phases = 1
    number_fluid_components = 1
  [../]

  [./pc]
    type = PorousFlowCapillaryPressureVG
    alpha = 1E-8
    m = 0.5
  [../]
[]
############################################################
[./FluidProperties]
  [./water]
    type = Water97FluidProperties
  [../]

  [./tabulated_water]
    type = TabulatedFluidProperties
    fp = water
    temperature_min = 275
    temperature_max = 450
    pressure_max = 1E8
    interpolated_properties = 'density viscosity enthalpy internal_energy'
    fluid_property_file = water97_tabulated.csv
  [../]
[../]
############################################################
[Materials]
  [./temperature]
    type = PorousFlowTemperature
    temperature = T
  [../]

  [./temperature_nodal]
    type = PorousFlowTemperature
    at_nodes = true
    temperature = T
  [../]

  [./ppss]
    type = PorousFlow1PhaseP
    at_nodes = true
    porepressure = P
    capillary_pressure = pc
  [../]

  [./ppss_qp]
    type = PorousFlow1PhaseP
    porepressure = P
    capillary_pressure = pc
  [../]

  [./massfrac]
    type = PorousFlowMassFraction
    at_nodes = true
  [../]

  [./fluid_props]
    type = PorousFlowSingleComponentFluid
    fp = water
    phase = 0
    at_nodes = true
  [../]

  [./fluid_props_qp]
    type = PorousFlowSingleComponentFluid
    phase = 0
    fp = water
  [../]

  [./rock_heat]
    type = PorousFlowMatrixInternalEnergy
    specific_heat_capacity = 825.0
    density = 2875.0
  [../]

  [./thermal_conductivity]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '2.83 0 0  0 2.83 0  0 0 2.83'
  [../]

  [./permeability]
    type = PorousFlowPermeabilityConst
    permeability = '1.0E-16 0 0  0 1.0E-16 0  0 0 1.0E-16'
  [../]

  [./porosity_qp]
    type = PorousFlowPorosityConst
    porosity = 0.1
  [../]

  [./relperm]
    type = PorousFlowRelativePermeabilityCorey
    at_nodes = true
    n = 1
    phase = 0
  [../]

  [./relperm_qp]
    type = PorousFlowRelativePermeabilityCorey
    n = 1
    phase = 0
  [../]

  [./fp_mat]
    type = FluidPropertiesMaterialPT
    pressure = P
    temperature = T
    fp = water
  [../]
[]
############################################################
[Preconditioning]
#  active = 'preferred'
  active = 'original'

  [./original]
    type = SMP
    full = true
    petsc_options = '-ksp_diagonal_scale -ksp_diagonal_scale_fix'
    petsc_options_iname = '-ksp_type -pc_type -sub_pc_type -sub_pc_factor_shift_type'
    petsc_options_value = 'gmres asm lu NONZERO'
  [../]

  [./superlu]
    type = SMP
    full = true
    petsc_options = '-ksp_diagonal_scale -ksp_diagonal_scale_fix'
    petsc_options_iname = '-ksp_type -pc_type -pc_factor_mat_solver_package'
    petsc_options_value = 'gmres lu superlu_dist'
  [../]

  [./sub_pc_superlu]
    type = SMP
    full = true
    petsc_options = '-ksp_diagonal_scale -ksp_diagonal_scale_fix'
    petsc_options_iname = '-ksp_type -pc_type -sub_pc_type -sub_pc_factor_shift_type -sub_pc_factor_mat_solver_package'
    petsc_options_value = 'gmres asm lu NONZERO superlu_dist'
  [../]

  [./preferred]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
    petsc_options_value = ' lu       mumps'
  [../]
[]
############################################################
[Postprocessors]

[]
############################################################
[Executioner]
  type = Transient
#  type = Steady

#[./TimeStepper]
#  type = SolutionTimeAdaptiveDT
#  dt = 50
#[../]

  solve_type = Newton
  end_time = 1
   dt = 1
   nl_abs_tol = 1e-6
   l_max_its  = 50
   l_tol      = 1e-6
   nl_max_its = 10
[]
############################################################
[Outputs]
  exodus  = true

   [./console]
    type = Console
    output_linear = true
    output_nonlinear = true
    verbose = true
  [../]


  [pgraph]
    type = PerfGraphOutput
    execute_on = 'initial final'  # Default is "final"
    level = 2                     # Default is 1
    heaviest_branch = true        # Default is false
    heaviest_sections = 5         # Default is 0
  []



  [./CSV]
    type = CSV
  [../]
[]
############################################################
