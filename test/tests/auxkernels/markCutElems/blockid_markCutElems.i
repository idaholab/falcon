[AuxKernels]
  [ls]
    type = MarkCutElems
    mesh_file = Cluster_34.exo
    variable = c
    uncut_value = 0
    use_block_ids = true
    execute_on = timestep_end
  []
[]
