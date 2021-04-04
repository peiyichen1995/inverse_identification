[Variables]
  [x]
    initial_value = 2
  []
  [y]
    initial_value = 5
  []
[]

[AuxVariables]
  [lambda]
  []
[]

[Objective]
  type = Rosenbrock
  a = 1
  b = 100
  x = x
  y = y
[]

[Solver]
  type = PetscTao
[]
