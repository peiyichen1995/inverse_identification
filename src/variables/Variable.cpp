#include "Variable.hpp"

Variable::Variable(Problem * problem, hit::Node * params)
  : Object(problem, params),
    _value(params->paramOptional<Real>("initial_value", 0)),
    _lower_bound(params->paramOptional<Real>("lower_bound", std::numeric_limits<Real>::min())),
    _upper_bound(params->paramOptional<Real>("upper_bound", std::numeric_limits<Real>::max()))
{
}

void
Variable::print(std::ostream & os) const
{
  os << Utils::indent(_indent) << name() << " = " << raw_value(_value);
}
