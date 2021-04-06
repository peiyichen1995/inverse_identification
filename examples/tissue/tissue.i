sample = 1
angle_1 = 21
angle_2 = 111

[Variables]
  [mu]
    initial_value = 5
    lower_bound = 0
  []
  [beta]
    initial_value = 10
    lower_bound = 0
  []
  [rho]
    initial_value = 0.25
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
    file = 'data/sample_${sample}_${angle_1}.csv'
    angle = ${angle_1}
  []
  [data2]
    type = TissueUniaxialTestData
    file = 'data/sample_${sample}_${angle_2}.csv'
    angle = ${angle_2}
  []
  [peggy]
    type = PeggyModel
    mu = mu
    beta = beta
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
  petsc_options = '-tao_monitor'
  petsc_option_names = ' -tao_type -tao_gatol -tao_grtol'
  petsc_option_values = 'blmvm     1e-4       1e-4'
[]
