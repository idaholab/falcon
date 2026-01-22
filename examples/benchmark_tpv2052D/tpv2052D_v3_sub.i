# Verification of Benchmark Problem TPV205-2D from the SCEC Dynamic Rupture Validation exercises #
# Reference: #
# Harris, R. M.-P.-A. (2009). The SCEC/USGS Dynamic Earthquake Rupture Code Verification Exercise. Seismological Research Letters, vol. 80, no. 1, pages 119-126. #

# [Version 3] #
# This file serves to: #
# Generalize the computation of sticking traction using consistent displacement jump and nodal reaction forces #

[Mesh]
    [./msh]
      type = GeneratedMeshGenerator
      dim = 2
      nx = 1000
      ny = 1000
      xmin = -50000
      xmax = 50000
      ymin = -50000
      ymax = 50000
    []
    [./new_block]
      type = ParsedSubdomainMeshGenerator
      input = msh
      combinatorial_geometry = 'y<0'
      block_id = 1
    []
    [./split]
      type = BreakMeshByBlockGenerator
      input = new_block
      split_interface = true
    []
    [interface]
        type = SideSetsBetweenSubdomainsGenerator
        input = split
        primary_block = 0
        paired_block = 1
        new_boundary = 'interface'
    []
    [secondary_interface]
        type = SideSetsBetweenSubdomainsGenerator
        input = interface
        primary_block = 1
        paired_block = 0
        new_boundary = 'secondary_interface'
    []
[]

[Variables]
    [disp_sub_x]
        order = FIRST
        family = LAGRANGE
    []
    [disp_sub_y]
        order = FIRST
        family = LAGRANGE
    []
[]

[AuxVariables]
    [./resid_sub_x]
        order = FIRST
        family = LAGRANGE
    [../]
    [./resid_sub_y]
        order = FIRST
        family = LAGRANGE
    [../]
[]

[GlobalParams]
    displacements = 'disp_sub_x disp_sub_y'
[]

[Modules]
    [./TensorMechanics]
      [./Master]
        [./all]
          strain = SMALL
          add_variables = true
          planar_formulation = PLANE_STRAIN
          generate_output = 'stress_xx stress_yy stress_xy'
          save_in = 'resid_sub_x resid_sub_y'
        [../]
      [../]
    [../]
[]

[Materials]
    [elasticity]
        type = ComputeIsotropicElasticityTensor
        lambda = 32.04e9
        shear_modulus = 32.04e9
        use_displaced_mesh = false
    []
    [stress]
        type = ComputeLinearElasticStress
    []
[]

[Executioner]
    type = Transient
    [TimeIntegrator]
        type = CentralDifference
        solve_type = lumped
    []
[]
