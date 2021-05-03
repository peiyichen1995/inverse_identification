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
  ADReal error(unsigned int i);
  void write(const std::string file_name,
             const std::vector<Real> & x,
             const std::vector<Real> & y) const;

  const HyperelasticModel * _hyperelastic_model;

  const ADReal & _lambda_2;
  const VariableName _lambda_2_name;

  const bool _postprocess;
  const std::vector<std::string> _file_names;

  std::vector<const TissueUniaxialTestData *> _datas;
};
