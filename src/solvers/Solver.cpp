#include "Solver.hpp"

Solver::Solver(Problem * problem, hit::Node * params) : Object(problem, params), Coupleable(this) {}
