#pragma once

#include "Problem.hpp"
#include "Variable.hpp"

class Problem;
class UserObject;

class Coupleable
{
public:
  Coupleable(Problem * problem) : _problem(problem) {}

  const ADReal & coupledValue(const VariableName name) { return _problem->variable(name)->value(); }

  DofId coupledDof(const VariableName name) { return _problem->dofMap().at(name); }

  template <typename T>
  const T * coupledUserObject(const UserObjectName name)
  {
    T * uo = dynamic_cast<T *>(_problem->userObject(name));
    return uo;
  }

  Problem * _problem;
};
