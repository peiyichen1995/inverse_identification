[Variables]
  [mu1]
    initial_value = 1e-6
    lower_bound = 1e-6
  []
  [mu2]
    initial_value = 1e-6
    lower_bound = 1e-6
  []
  [mu4]
    initial_value = 4.42043
    lower_bound = 1e-6
  []
  [beta4]
    initial_value = 11.2437
    lower_bound = 1e-6
  []
  [rho]
    initial_value = 0.260694
    lower_bound = 0
    upper_bound = 1
  []
[]

[AuxVariables]
  [lambda_2]
  []
[]

[UserObjects]
  [data1]
    type = TissueUniaxialTestData
    file = 'data/${layer}/sample_${sample}_${angle_1}.csv'
    angle = ${angle_1}
  []
  [data2]
    type = TissueUniaxialTestData
    file = 'data/${layer}/sample_${sample}_${angle_2}.csv'
    angle = ${angle_2}
  []
  [peggy]
    type = RudyModel
    mu1 = mu1
    mu2 = mu2
    mu4 = mu4
    beta4 = beta4
    rho = rho
  []
[]

[Objective]
  type = TissueUniaxialTestDataMismatch
  test_data = 'data1 data2'
  hyperelastic_model = peggy
  lambda_2 = lambda_2
[]

[Solver]
  type = PetscTao
  petsc_options = '-tao_cmonitor'
  petsc_option_names = ' -tao_type -tao_gatol -tao_gttol -tao_max_it'
  petsc_option_values = 'blmvm     1e-3       1e-3       50'
[]
