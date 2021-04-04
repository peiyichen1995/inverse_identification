#include "Solver.hpp"

Solver::Solver(Problem * problem, hit::Node * params) : Object(params), Coupleable(problem) {}
