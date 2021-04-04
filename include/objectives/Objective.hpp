#pragma once

#include "Object.hpp"
#include "Coupleable.hpp"

class Objective : public Object, public Coupleable
{
public:
  Objective(Problem * problem, hit::Node * params);
  virtual ~Objective() {}

  virtual ADReal value() = 0;

protected:
private:
};
