all_frac_ids = "fracture1 fracture2 fracture3"

frac_aperture_1 = ${frac_aperature}
frac_aperture_2 = ${frac_aperature}
frac_aperture_3 = ${frac_aperature}

frac_roughness_1 = ${frac_roughness}
frac_roughness_2 = ${frac_roughness}
frac_roughness_3 = ${frac_roughness}

one_over_bulk = ${fparse 1/70e9}

[Materials]
  [porosity_fracture1]
    type = PorousFlowPorosityLinear
    porosity_ref = ${frac_aperture_1}
    P_ref = initial_p
    P_coeff = ${one_over_bulk}
    porosity_min = ${fparse frac_aperture_1/10}
    block = fracture1
  []
  [porosity_fracture2]
    type = PorousFlowPorosityLinear
    porosity_ref = ${frac_aperture_2}
    P_ref = initial_p
    P_coeff = ${one_over_bulk}
    porosity_min = ${fparse frac_aperture_2/10}
    block = fracture2
  []
  [porosity_fracture3]
    type = PorousFlowPorosityLinear
    porosity_ref = ${frac_aperture_3}
    P_ref = initial_p
    P_coeff = ${one_over_bulk}
    porosity_min = ${fparse frac_aperture_3/10}
    block = fracture3
  []

  [permeability_fracture1]
    type = PorousFlowPermeabilityKozenyCarman
    k0 = ${fparse frac_roughness_1/12*frac_aperture_1^3}
    poroperm_function = kozeny_carman_phi0
    m = 0
    n = 3
    phi0 = ${frac_aperture_1}
    block = fracture1
  []
  [permeability_fracture2]
    type = PorousFlowPermeabilityKozenyCarman
    k0 = ${fparse frac_roughness_2/12*frac_aperture_2^3}
    poroperm_function = kozeny_carman_phi0
    m = 0
    n = 3
    phi0 = ${frac_aperture_2}
    block = fracture2
  []
  [permeability_fracture3]
    type = PorousFlowPermeabilityKozenyCarman
    k0 = ${fparse frac_roughness_3/12*frac_aperture_3^3}
    poroperm_function = kozeny_carman_phi0
    m = 0
    n = 3
    phi0 = ${frac_aperture_3}
    block = fracture3
  []

  [rock_internal_energy_fracture]
    type = PorousFlowMatrixInternalEnergy
    density = 2500
    specific_heat_capacity = 100.0
    block = ${all_frac_ids}
  []

  [thermal_conductivity_fracture]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '3 0 0 0 3 0 0 0 3'
    block = ${all_frac_ids}
  []
[]
