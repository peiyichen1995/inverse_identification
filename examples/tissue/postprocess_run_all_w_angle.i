[Variables]
  [mu1]
    initial_value = ${mu1}
  []
  [mu2]
    initial_value = ${mu2}
  []
  [mu4]
    initial_value = ${mu4}
  []
  [beta4]
    initial_value = ${beta4}
  []
  [rho]
    initial_value = ${rho}
  []
[]
[AuxVariables]
  [lambda_2]
  []
[]
[UserObjects]
  [data1]
    type = TissueUniaxialTestData
    file = 'data/${layer}/sample_1_69.5.csv'
    angle = ${angle_1}
  []
  [data2]
    type = TissueUniaxialTestData
    file = 'data/${layer}/sample_11_156.3.csv'
    angle = ${angle_2}
  []
  [model]
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
  hyperelastic_model = model
  lambda_2 = lambda_2
  postprocess = true
  file_names = 'output/${fileName}/sample_${sample}_angle_1_${N}.csv '
               'output/${fileName}/sample_${sample}_angle_2_${N}.csv'
[]
[Solver]
  type = PetscTao
  petsc_options = '-tao_monitor'
  petsc_option_names = ' -tao_type -tao_gatol -tao_grtol'
  petsc_option_values = 'blmvm     1e-4       1e-4'
[]
