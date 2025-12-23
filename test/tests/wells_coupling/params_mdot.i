# mass flow ratios do not sum to one, so we renormalize to use a reduced
# total flow rate with updated mass ratios:
inj1_mass_ratio_unnormalized = 0.074
inj2_mass_ratio_unnormalized = 0.101
inj3_mass_ratio_unnormalized = 0.258
total_mass_ratio = ${fparse inj1_mass_ratio_unnormalized + inj2_mass_ratio_unnormalized + inj3_mass_ratio_unnormalized}

injection_ramp_duration = 10000
