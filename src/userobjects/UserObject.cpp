#include "UserObject.hpp"

UserObject::UserObject(Problem * problem, hit::Node * params) : Object(params), Coupleable(problem)
{
}
