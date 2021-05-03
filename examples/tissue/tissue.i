sample = 10
angle_1 = 53.4
angle_2 = 143.4

[Variables]
  [mu1]
    initial_value = 0
    lower_bound = 0
  []
  [mu2]
    initial_value = 1
    lower_bound = 0
  []
  [mu4]
    initial_value = 20
    lower_bound = 0
  []
  [beta4]
    initial_value = 100
    lower_bound = 0
  []
  [rho]
    initial_value = 0.5
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
    file = 'data/adventitia/sample_${sample}_${angle_1}.csv'
    angle = ${angle_1}
  []
  [data2]
    type = TissueUniaxialTestData
    file = 'data/adventitia/sample_${sample}_${angle_2}.csv'
    angle = ${angle_2}
  []
  # [peggy]
  #   type = PeggyModel
  #   mu = mu4
  #   beta = beta4
  #   rho = rho
  # []
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
  petsc_option_names = ' -tao_type -tao_gatol -tao_gttol'
  petsc_option_values = 'blmvm     5e-3       5e-3      '
[]
