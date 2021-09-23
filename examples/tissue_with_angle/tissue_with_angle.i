[Variables]
  [mu1]
    initial_value = 7
    lower_bound = 1e-6
  []
  [mu2]
    initial_value = 4
    lower_bound = 1e-6
  []
  [mu4]
    initial_value = 5
    lower_bound = 1e-6
  []
  [beta4]
    initial_value = 5
    lower_bound = 1e-6
  []
  [rho]
    initial_value = 0.3
    lower_bound = 0
    upper_bound = 1
  []
  [angle]
    initial_value = 0.45
    lower_bound = 0
    upper_bound = 1.57
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
    add_half_pi = true
  []
  [Gary]
    type = RudyModel
    mu1 = mu1
    mu2 = mu2
    mu4 = mu4
    beta4 = beta4
    rho = rho
  []
[]

[Objective]
  type = TissueUniaxialTestDataMismatchWithAngle
  test_data = 'data1 data2'
  hyperelastic_model = Gary
  lambda_2 = lambda_2
  angle = angle
[]

[Solver]
  type = PetscTao
  petsc_options = '-tao_cmonitor'
  petsc_option_names = ' -tao_type -tao_gatol -tao_gttol -tao_max_it'
  petsc_option_values = 'blmvm     1e-3       1e-3       50'
[]
