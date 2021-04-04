#include "Property.hpp"

template <typename T>
Property<T>::Property(Problem * problem, hit::Node * params) : Object(params), Coupleable(problem)
{
}

template <typename T>
std::ostream &
operator<<(std::ostream & os, const Property<T> & v)
{
  os << std::setw(OBJECT_NAME_MAX_LENGTH) << v.name() << " = " << v._v;
  return os;
}
