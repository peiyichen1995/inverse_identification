#include "UserObject.hpp"

UserObject::UserObject(Problem * problem, hit::Node * params)
  : Object(problem, params), Coupleable(this)
{
}
