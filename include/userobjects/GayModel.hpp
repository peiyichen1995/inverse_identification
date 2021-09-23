#pragma once

#include "HyperelasticModel.hpp"

class GayModel : public HyperelasticModel
{
public:
  GayModel(Problem * problem, hit::Node * params);

  virtual ADRankTwoTensor PK2(const ADRankTwoTensor & F, void * context = nullptr) const override;

protected:
private:
  registerObjectHpp(GayModel);

  const ADReal & _mu1;
  const ADReal & _mu2;
  const ADReal & _mu4;
  const ADReal & _beta4;
  const ADReal & _rho;
  const ADReal & _angle;
};
