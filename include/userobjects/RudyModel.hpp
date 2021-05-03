#pragma once

#include "HyperelasticModel.hpp"

class RudyModel : public HyperelasticModel
{
public:
  RudyModel(Problem * problem, hit::Node * params);

  virtual ADRankTwoTensor PK2(const ADRankTwoTensor & F, void * context = nullptr) const override;

protected:
private:
  registerObjectHpp(RudyModel);

  Real _angle;
  const ADReal & _mu1;
  const ADReal & _mu2;
  const ADReal & _mu4;
  const ADReal & _beta4;
  const ADReal & _rho;
};
