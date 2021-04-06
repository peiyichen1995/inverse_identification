#pragma once

#include "HyperelasticModel.hpp"

class PeggyModel : public HyperelasticModel
{
public:
  PeggyModel(Problem * problem, hit::Node * params);

  virtual ADRankTwoTensor PK2(const ADRankTwoTensor & F, void * context = nullptr) const override;

protected:
private:
  registerObjectHpp(PeggyModel);

  Real _angle;
  const ADReal & _mu;
  const ADReal & _beta;
  const ADReal & _rho;
};
