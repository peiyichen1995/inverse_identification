#pragma once

#include "Types.hpp"
#include "Problem.hpp"

class HyperelasticModel
{
public:
  HyperelasticModel(Problem * problem);
  virtual ~HyperelasticModel() {}

  // Calculate the second Piola-Kirchhof stress given a deformation gradient
  virtual ADRankTwoTensor PK2(const ADRankTwoTensor & F) = 0;

protected:
  Problem * _problem;

private:
};
