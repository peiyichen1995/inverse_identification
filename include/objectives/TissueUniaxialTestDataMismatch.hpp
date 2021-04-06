#pragma once

#include "Objective.hpp"
#include "TissueUniaxialTestData.hpp"
#include "HyperelasticModel.hpp"

class TissueUniaxialTestDataMismatch : public Objective
{
public:
  TissueUniaxialTestDataMismatch(Problem * problem, hit::Node * params);

  virtual ADReal value() override;

protected:
private:
  registerObjectHpp(TissueUniaxialTestDataMismatch);

  ADReal uniaxialStress(Real lambda_1, Real angle);
  ADReal error(const TissueUniaxialTestData * data);

  const HyperelasticModel * _hyperelastic_model;

  const ADReal & _lambda_2;
  const VariableName _lambda_2_name;

  std::vector<const TissueUniaxialTestData *> _datas;
};
