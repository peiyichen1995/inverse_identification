[Variables]
  [x]
    initial_value = 0
  []
  [y]
    initial_value = 2
  []
[]

[AuxVariables]
[]

[UserObjects]
[]

[Objective]
  type = Rosenbrock
  x = x
  y = y
  a = 1
  b = 100
[]

[Solver]
  type = PetscTao
  petsc_options = '-tao_monitor'
  petsc_option_names = ' -tao_type -tao_gatol -tao_grtol'
  petsc_option_values = 'nls       1e-4       1e-4'
[]
