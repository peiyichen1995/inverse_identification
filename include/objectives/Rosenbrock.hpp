#pragma once

#include "Objective.hpp"

class Rosenbrock : public Objective
{
public:
  Rosenbrock(Problem * problem, hit::Node * params);

  virtual ADReal value() override;

protected:
private:
  registerObjectHpp(Rosenbrock);

  const ADReal & _x;
  const ADReal & _y;
  const Real _a;
  const Real _b;
};
