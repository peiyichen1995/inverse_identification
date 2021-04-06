## A lightweight optimization framework

I developed this framework for inverse identification of elasticity parameters for soft biological tissues given experimental data.

This framework relies on
- `Hit` for input file parsing,
- `MetaPhysicL` for automatic differentiation (to obtain gradient and Hessian),
- `Eigen` for matrix/vector algebra, and most importantly
- `PETSc/TAO` for optimization algorithms.

### Example 1: Rosenbrock

```
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
```

The input file is mostly self-explanatory. The definition of the Rosenbrock function can be found in `src/objectives/Rosenbrock.cpp`:

```cpp
#include "Rosenbrock.hpp"

registerObjectCpp(Rosenbrock);

Rosenbrock::Rosenbrock(Problem * problem, hit::Node * params)
  : Objective(problem, params),
    _x(coupledValue(params->param<VariableName>("x"))),
    _y(coupledValue(params->param<VariableName>("y"))),
    _a(params->param<Real>("a")),
    _b(params->param<Real>("b"))
{
}

ADReal
Rosenbrock::value()
{
  return std::pow(_a - _x, 2) + _b * std::pow(_y - _x * _x, 2);
}
```

### Example 2: Tissue parameter identification

This is a bound constrained optimization problem minimizing the mean-squared-error between the model prediction and the experimental data.

```
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
```
