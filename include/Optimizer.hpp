#pragma once

#include "Types.hpp"
#include <petsctao.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Optimizer
{
public:
  Optimizer(std::array<Real, NVARS> values,
            const std::vector<std::pair<ADReal, ADReal>> lambda1s,
            const std::vector<ADReal> sigma_exp);

  Gradient getGradient(ADReal f, std::vector<unsigned int> var_indices);
  Hessian getHessian(ADReal f, std::vector<unsigned int> var_indices);

  ADReal objective();
  ADReal objective(const PetscReal * x);
  ADRankTwoTensor mult(const ADRankTwoTensor &, const ADRankTwoTensor &);
  ADRankTwoTensor cross(const ADRankOneTensor &, const ADRankOneTensor &);

protected:
private:
  ADReal stress(ADReal lambda1, ADReal angle);
  ADRankTwoTensor PK2(ADReal lambda1, ADReal angle);

  const std::vector<std::pair<ADReal, ADReal>> _lambda1s;
  const std::vector<ADReal> _sigma_exp;
  std::array<ADReal, NVARS> _variables;
};
