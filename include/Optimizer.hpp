#pragma once

#include "Types.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Optimizer
{
public:
  Optimizer(std::array<Real, NVARS> values,
            const std::vector<ADReal> lambda1s,
            const std::vector<ADReal> sigma_exp);

  void identifyConstraints(Gradient LM,
                           Gradient step,
                           std::vector<unsigned int> & A,
                           std::vector<unsigned int> & I);
  void optimizeNonnegative(std::vector<unsigned int> var_indices);

  void optimize(std::vector<unsigned int> var_indices);

  Gradient getGradient(ADReal f, std::vector<unsigned int> var_indices);
  Hessian getHessian(ADReal f, std::vector<unsigned int> var_indices);

  Gradient getGradientOnInactiveSet(Gradient g, const std::vector<unsigned int> & I);
  Hessian getHessianOnInactiveSet(Hessian H, const std::vector<unsigned int> & I);

  void takeStep(Gradient step, const std::vector<unsigned int> & var_indices);
  void takeStep(Gradient step,
                const std::vector<unsigned int> & var_indices,
                const std::vector<unsigned int> & I);

  void printVariables(std::vector<unsigned int> var_indices);

  ADReal objective();
  ADRankTwoTensor mult(const ADRankTwoTensor &, const ADRankTwoTensor &);
  ADRankTwoTensor cross(const ADRankOneTensor &, const ADRankOneTensor &);

protected:
private:
  ADReal stress(ADReal lambda1);
  ADRankTwoTensor PK2(ADReal lambda1);

  const std::vector<ADReal> _lambda1s;
  const std::vector<ADReal> _sigma_exp;
  std::array<ADReal, NVARS> _variables;
};
