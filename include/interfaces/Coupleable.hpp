#pragma once

#include "Problem.hpp"
#include "Variable.hpp"

class Problem;
class UserObject;

class Coupleable
{
public:
  Coupleable(Object * object) : _object(object) {}

  const ADReal & coupledValue(const VariableName name)
  {
    return _object->problem()->variable(name)->value();
  }

  DofId coupledDof(const VariableName name) { return _object->problem()->dofMap().at(name); }

  template <typename T>
  const T * coupledUserObject(const UserObjectName name)
  {
    T * uo = dynamic_cast<T *>(_object->problem()->userObject(name));
    return uo;
  }

private:
  Object * _object;
};
