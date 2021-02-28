#pragma once

#include "Problem.hpp"
#include "Types.hpp"

class MinimizationProblem : public Problem
{
public:
  MinimizationProblem() : Problem() {}
  virtual ~MinimizationProblem() {}

  // Compute the objective function to be minimized, along with its gradient with respect to all the
  // variables.
  virtual std::pair<Real, Vector> computeObjectiveAndGradient(const Vector & x) = 0;

protected:
  Vector getGradient(ADReal & f) const;
  Vector getGradient(ADReal & f, const std::vector<unsigned int> indices) const;

  Matrix getHessian(ADReal & f) const;
  Matrix getHessian(ADReal & f, const std::vector<unsigned int> indices) const;

private:
};
