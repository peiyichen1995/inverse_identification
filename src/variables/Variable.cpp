#include "Variable.hpp"

Variable::Variable(hit::Node * params)
  : Object(params), _value(params->paramOptional<Real>("initial_value", 0))
{
}

std::ostream &
operator<<(std::ostream & os, const Variable & v)
{
  os << v.name() << " = " << raw_value(v._value);
  return os;
}
