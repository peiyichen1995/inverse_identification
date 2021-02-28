#pragma once

#include "HyperelasticModel.hpp"

class PeggyModel : public HyperelasticModel
{
public:
  PeggyModel(Problem * problem);

  virtual ADRankTwoTensor PK2(const ADRankTwoTensor & F) override;

protected:
private:
  const ADReal & _mu4;
  const ADReal & _beta4;
  const ADReal & _rho;
  const ADReal & _angle;
};
