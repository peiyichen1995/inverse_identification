#pragma once

#include "Object.hpp"
#include "Coupleable.hpp"

class Solver : public Object, public Coupleable
{
public:
  Solver(Problem * problem, hit::Node * params);
  virtual ~Solver() {}

  virtual int solve() = 0;

protected:
private:
};
