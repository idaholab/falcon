[Mesh]
  dim = 3
  file = 1d_x.e
[]

[Variables]
  active = 'pressure temperature'
# active = 'pressure'

   [./pressure]
    order = FIRST
    family = LAGRANGE
   initial_condition = 100000.0
   [../]

  [./temperature]
    order = FIRST
    family = LAGRANGE
    initial_condition = 20
   [../]
[]

[AuxVariables]
  active = 'v_x density_water viscosity_water porosity'

  [./v_x]
# velocity aux kernels must be elemental, coupled to _grad_p
    order = CONSTANT
    family = MONOMIAL
  [../]

[./density_water]
    order = FIRST
    family = LAGRANGE
  [../]

[./viscosity_water]
    order = FIRST
    family = LAGRANGE
  [../]

#use this for nodal version 
[./porosity]
	order = FIRST
	family = LAGRANGE
	initial_condition = 0.25
[../]
 
#this is for material version
# [./porosity]
# order = CONSTANT
# family = MONOMIAL
# initial_condition = 0.4
# [../] 
[]

 
[Kernels]
  active = 'p_wmfp t_d'
#  active = 'p_ie p_dmfp p_dmfz t_ie t_d t_c'
#  active = 'p_wmfp dv_x'


  [./p_wmfp]
    type = WaterMassFluxPressure
    variable = pressure
  [../]

  [./t_d]
    type = TemperatureDiffusion
    variable = temperature
 
  [../]

[]

 [AuxKernels]
 active = 'velocity_x density_water viscosity_water porosity_nodal'
  
  [./velocity_x]
    type = VelocityAux
    variable = v_x
    pressure = pressure
    density_water = density_water
    porosity = porosity
    component = 0
  [../]

  [./density_water]
    type = CoupledDensityAux
    variable = density_water
    temperature = temperature
    pressure = pressure
    density_water = 999
    temp_dependent_density = true
  [../]

  [./viscosity_water]
    type = CoupledViscosityAux
    variable = viscosity_water
    temperature = temperature
	pressure = pressure
    viscosity_water = 0.001
    temp_dependent_viscosity = true
  [../]

 [./porosity_nodal]
	type = CoupledPorosityNodalAux
	variable = porosity
	porosity = 0.25
 [../]
 
 [./porosity_material]
	type = CoupledPorosityMaterialAux
	variable = porosity
 [../]
[]

 
[BCs]
  active = 'left_p right_p center_t left_t right_t'
#  active = 'left_p right_p'

  [./left_p]
    type = DirichletBC
    variable = pressure
    boundary = 1
    value = 110000.0
  [../]

  [./right_p]
    type = DirichletBC
    variable = pressure
    boundary = 2
    value = 100000.0
  [../]
 
 [./center_t]
	type = DirichletBC
 	variable = temperature
 	boundary = 3
 	value = 21.0
 [../]
 
 [./left_t]
	type = DirichletBC
 	variable = temperature
 	boundary = 1
	 value = 20.0
 [../]
 
 [./right_t]
	type = DirichletBC
 	variable = temperature
 	boundary = 2
 	value = 20.0
 [../]
  
[]


[Materials]
  active = 'PorousMedia'
  
	[./PorousMedia]
 type = Geothermal

 block = 1
 
#fluid flow
  #	temp_dependent_density = false
	pressure = pressure
 	density_water = density_water
	viscosity_water = viscosity_water
	porosity = porosity

	gravity              =  9.8
	permeability         =  1.0e-12
	compressibility      =  4.6e-10
 

 
#heat transprt
	temperature = temperature
 
	density_rock         =  2500
	thermal_conductivity =  2.5
    	specific_heat_water  =	4186
    	specific_heat_rock   =  920

 
 
#mechanics 
	has_solid_mechanics   = false

#    	x_disp = x_disp
#   	y_disp = y_disp
#    	z_disp = z_disp
 
#	thermal_expansion    =   2.0e-6
#    	youngs_modulus       =   1.50e10
#    	poissons_ratio       =   0.3
#    	biot_coeff           =   0.2
#    	t_ref                =  200.0


  [../]

[ ]


[Executioner]
  active = ' '	
  type = Steady
  petsc_options = '-snes_mf'
#  petsc_options = '-snes_mf_operator'
[]

[Output]
  file_base = out
  output_initial = true
  elemental_as_nodal = true
  interval = 1
  exodus = true
  perf_log = true
[]
    
