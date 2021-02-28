#include "MinimizationProblem.hpp"

Vector
MinimizationProblem::getGradient(ADReal & f) const
{
  Vector g(_variables.size());
  for (unsigned int i = 0; i < _variables.size(); i++)
    g[i] = f.value().derivatives()[i];
  return g;
}

Vector
MinimizationProblem::getGradient(ADReal & f, const std::vector<unsigned int> indices) const
{
  Vector g(indices.size());
  for (unsigned int i = 0; i < indices.size(); i++)
    g[i] = f.value().derivatives()[indices[i]];
  return g;
}

Matrix
MinimizationProblem::getHessian(ADReal & f) const
{
  Hessian H(_variables.size(), _variables.size());
  for (unsigned int i = 0; i < _variables.size(); i++)
    for (unsigned int j = 0; j < _variables.size(); j++)
      H(i, j) = f.derivatives()[i].derivatives()[j];
  return H;
}

Matrix
MinimizationProblem::getHessian(ADReal & f, const std::vector<unsigned int> indices) const
{
  Hessian H(indices.size(), indices.size());
  for (unsigned int i = 0; i < indices.size(); i++)
    for (unsigned int j = 0; j < indices.size(); j++)
      H(i, j) = f.derivatives()[indices[i]].derivatives()[indices[j]];
  return H;
}
