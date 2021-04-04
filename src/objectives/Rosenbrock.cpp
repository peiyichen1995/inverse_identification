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
