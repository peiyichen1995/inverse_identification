#pragma once

#include "MinimizationProblem.hpp"
#include "PeggyModel.hpp"

class TissueParameterIdentification : public MinimizationProblem
{
public:
  TissueParameterIdentification() {}

  virtual ~TissueParameterIdentification() {}

  void addHyperelasticModel(HyperelasticModel * model) { _model = model; }

  void addExperimentalData(const char * file_name);

  void printExperimentalData(std::ostream & os = std::cout) const;

  virtual std::pair<Real, Vector> computeObjectiveAndGradient(const Vector & x) override;

protected:
private:
  ADReal computeStress();

  HyperelasticModel * _model;

  typedef std::vector<std::tuple<Real, Real, Real>> ExperimentalData;

  std::vector<ExperimentalData> _data;
};
