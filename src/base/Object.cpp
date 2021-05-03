#include "Object.hpp"
#include "Problem.hpp"

Object::Object(Problem * problem, hit::Node * params)
  : _problem(problem),
    _indent(problem->indent()),
    _name(params->path()),
    _type(params->paramOptional<std::string>("type", "None"))
{
}

void
Object::print(std::ostream & os) const
{
  os << Utils::indent(_indent) << _name << ": " << std::endl;
  _indent++;
  os << Utils::indent(_indent) << "Type: " << _type;
}

std::ostream &
operator<<(std::ostream & os, const Object & o)
{
  o.print(os);
  return os;
}
