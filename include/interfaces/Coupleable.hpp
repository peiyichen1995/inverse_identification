#pragma once

#include "Problem.hpp"
#include "Variable.hpp"

class Problem;

class Coupleable
{
public:
  Coupleable(Problem * problem) : _problem(problem) {}

  const ADReal & coupledValue(const VariableName name) { return _problem->variable(name)->value(); }

  Problem * _problem;
};
