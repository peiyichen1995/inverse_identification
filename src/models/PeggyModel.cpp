#include "PeggyModel.hpp"
#include "Utils.hpp"

PeggyModel::PeggyModel(Problem * problem)
  : HyperelasticModel(problem),
    _mu4(problem->getVariableValue("mu_4")),
    _beta4(problem->getVariableValue("beta_4")),
    _rho(problem->getVariableValue("rho")),
    _angle(problem->getVariableValue("tissue_angle"))
{
}

ADRankTwoTensor
PeggyModel::PK2(const ADRankTwoTensor & F)
{
  // Identity tensor
  ADRankTwoTensor I2;
  I2.setIdentity();

  // Tissue orientation
  ADReal angle = _angle * M_PI / 180;
  ADRankOneTensor a1(std::cos(angle), -std::sin(angle), 0);
  ADRankOneTensor a2(std::cos(angle), std::sin(angle), 0);
  ADRankTwoTensor M1 = Utils::cross(a1, a1);
  ADRankTwoTensor M2 = Utils::cross(a2, a2);

  // Project the right Cauchy strain onto tissue orientation
  ADRankTwoTensor C = Utils::mult(F.transpose(), F);
  ADRankTwoTensor CM1 = Utils::mult(C, M1);
  ADRankTwoTensor CM2 = Utils::mult(C, M2);
  ADReal e1 = CM1.trace() - 1;
  ADReal e2 = CM2.trace() - 1;
  ADReal e1pos = (std::abs(e1) + e1) / 2;
  ADReal e2pos = (std::abs(e2) + e2) / 2;

  ADRankTwoTensor S_ti_1 =
      4 * _mu4 * ((1 - _rho) * (C.trace() - 3) * I2 + _rho * e1pos * M1) *
      std::exp(_beta4 * ((1 - _rho) * (C.trace() - 3) * (C.trace() - 3) + _rho * e1pos * e1pos));
  ADRankTwoTensor S_ti_2 =
      4 * _mu4 * ((1 - _rho) * (C.trace() - 3) * I2 + _rho * e2pos * M2) *
      std::exp(_beta4 * ((1 - _rho) * (C.trace() - 3) * (C.trace() - 3) + _rho * e2pos * e2pos));
  ADRankTwoTensor S = S_ti_1 + S_ti_2;

  return S;
}
