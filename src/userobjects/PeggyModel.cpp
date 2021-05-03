#include "PeggyModel.hpp"

registerObjectCpp(PeggyModel);

PeggyModel::PeggyModel(Problem * problem, hit::Node * params)
  : HyperelasticModel(problem, params),
    _mu(coupledValue(params->param<VariableName>("mu"))),
    _beta(coupledValue(params->param<VariableName>("beta"))),
    _rho(coupledValue(params->param<VariableName>("rho")))
{
}

ADRankTwoTensor
PeggyModel::PK2(const ADRankTwoTensor & F, void * context) const
{
  // Identity tensor
  ADRankTwoTensor I2;
  I2.setIdentity();

  // Tissue orientation
  Real angle = *static_cast<Real *>(context);
  ADRankOneTensor a1(std::cos(angle), -std::sin(angle), 0);
  ADRankOneTensor a2(std::cos(angle), std::sin(angle), 0);
  ADRankTwoTensor M1 = a1 * a1.transpose();
  ADRankTwoTensor M2 = a2 * a2.transpose();

  // Project the right Cauchy strain onto tissue orientation
  ADRankTwoTensor C = F.transpose() * F;
  ADRankTwoTensor CM1 = C * M1;
  ADRankTwoTensor CM2 = C * M2;
  ADReal e1 = CM1.trace() - 1;
  ADReal e2 = CM2.trace() - 1;
  ADReal e1pos = (std::abs(e1) + e1) / 2;
  ADReal e2pos = (std::abs(e2) + e2) / 2;

  ADRankTwoTensor S_ti_1 =
      4 * _mu * ((1 - _rho) * (C.trace() - 3) * I2 + _rho * e1pos * M1) *
      std::exp(_beta * ((1 - _rho) * (C.trace() - 3) * (C.trace() - 3) + _rho * e1pos * e1pos));
  ADRankTwoTensor S_ti_2 =
      4 * _mu * ((1 - _rho) * (C.trace() - 3) * I2 + _rho * e2pos * M2) *
      std::exp(_beta * ((1 - _rho) * (C.trace() - 3) * (C.trace() - 3) + _rho * e2pos * e2pos));
  ADRankTwoTensor S = S_ti_1 + S_ti_2;

  return S;
}
