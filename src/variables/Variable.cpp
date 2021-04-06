#include "Variable.hpp"

Variable::Variable(hit::Node * params)
  : Object(params),
    _value(params->paramOptional<Real>("initial_value", 0)),
    _lower_bound(params->paramOptional<Real>("lower_bound", std::numeric_limits<Real>::min())),
    _upper_bound(params->paramOptional<Real>("upper_bound", std::numeric_limits<Real>::max()))
{
}

std::ostream &
operator<<(std::ostream & os, const Variable & v)
{
  os << v.name() << " = " << raw_value(v._value);
  return os;
}
