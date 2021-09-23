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
  [angle]
    initial_value = ${angle}
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
  postprocess = true
  file_names = 'output/${layer}/sample_${sample}_${angle_1}_${N}.csv '
               'output/${layer}/sample_${sample}_${angle_2}_${N}.csv'
[]

[Solver]
  type = PetscTao
  petsc_options = '-tao_cmonitor'
  petsc_option_names = ' -tao_type -tao_gatol -tao_gttol -tao_max_it'
  petsc_option_values = 'blmvm     1e-3       1e-3       50'
[]
