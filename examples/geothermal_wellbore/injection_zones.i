# Carves a higher-permeability sub-zone out of the reservoir block, entirely within the open
# interval (y=-1000 to -2000), so that the injection well's flow partitioning across its
# open-interval points - which falls out for free from PorousFlowPeacemanBorehole's own
# well-index formula once a single bottomhole pressure is chosen (see
# PorousFlowRateControlledBoreholePressure.md) - is actually visible: with a uniform reservoir
# every open-interval point has the same well index, so there would be nothing to partition.
#
# Bounds are aligned to model_common.i's own 50m mesh (y=-1000 to -2000, ny=40 over 2000m) so
# geothermal_wellbore.bh's points at y=-1400, -1500, -1600 land strictly interior to the zone -
# no ambiguity about which element a Dirac point falls in.
#
# model_common.i's own materials are all block-restricted to 'cap'/'reservoir', so carving out
# this new block requires supplying its own full set of materials, not just permeability -
# porosity and thermal properties are left identical to the surrounding reservoir, so any
# difference in per-point flux is unambiguously attributable to permeability alone.

[Mesh]
  [high_k_zone]
    type = SubdomainBoundingBoxGenerator
    input = rename
    bottom_left = '0 -1650 -1'
    top_right = '1000 -1350 1'
    block_id = 3
  []
  [rename_high_k]
    type = RenameBlockGenerator
    input = high_k_zone
    old_block = 3
    new_block = 'reservoir_high_k'
  []
[]

[Materials]
  [porosity_reservoir_high_k]
    type = PorousFlowPorosityConst
    porosity = 0.1
    block = reservoir_high_k
  []
  [permeability_reservoir_high_k]
    # 10x the surrounding reservoir (1E-15 m^2) - a realistic contrast between an unusually
    # productive interval and the formation around it.
    type = PorousFlowPermeabilityConst
    permeability = '1E-14 0 0  0 1E-14 0  0 0 1E-14'
    block = reservoir_high_k
  []
  [thermal_conductivity_reservoir_high_k]
    type = PorousFlowThermalConductivityIdeal
    dry_thermal_conductivity = '2.5 0 0  0 2.5 0  0 0 2.5'
    block = reservoir_high_k
  []
  [rock_internal_energy_reservoir_high_k]
    type = PorousFlowMatrixInternalEnergy
    density = 2700
    specific_heat_capacity = 800
    block = reservoir_high_k
  []
[]
