#pragma once

#include "UserObject.hpp"

class HyperelasticModel : public UserObject
{
public:
  HyperelasticModel(Problem * problem, hit::Node * params);
  virtual ~HyperelasticModel() {}

  virtual ADRankTwoTensor PK2(const ADRankTwoTensor & F, void * context = nullptr) const = 0;

protected:
private:
};
