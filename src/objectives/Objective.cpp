#include "Objective.hpp"

Objective::Objective(Problem * problem, hit::Node * params)
  : Object(problem, params), Coupleable(this)
{
}
