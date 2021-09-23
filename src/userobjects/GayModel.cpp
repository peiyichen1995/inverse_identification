#include "GayModel.hpp"

registerObjectCpp(GayModel);

GayModel::GayModel(Problem * problem, hit::Node * params)
  : HyperelasticModel(problem, params),
    _mu1(coupledValue(params->param<VariableName>("mu1"))),
    _mu2(coupledValue(params->param<VariableName>("mu2"))),
    _mu4(coupledValue(params->param<VariableName>("mu4"))),
    _beta4(coupledValue(params->param<VariableName>("beta4"))),
    _rho(coupledValue(params->param<VariableName>("rho"))),
    _angle(coupledValue(params->param<VariableName>("angle")))
{
}

ADRankTwoTensor
GayModel::PK2(const ADRankTwoTensor & F, void * /*context*/) const
{
  // Identity tensor
  ADRankTwoTensor I2;
  I2.setIdentity();

  // Tissue orientation
  ADRankOneTensor a1(std::cos(_angle), -std::sin(_angle), 0);
  ADRankOneTensor a2(std::cos(_angle), std::sin(_angle), 0);
  ADRankTwoTensor M1 = a1 * a1.transpose();
  ADRankTwoTensor M2 = a2 * a2.transpose();

  ADReal J = F.determinant();
  ADRankTwoTensor C = F.transpose() * F;
  ADReal Jdev = std::pow(J, -2.0 / 3.0);
  ADRankTwoTensor C_bar = Jdev * C;
  ADRankTwoTensor C_bar_inv = C_bar.inverse();
  ADRankTwoTensor C_bar_cof = C_bar.determinant() * C_bar_inv;
  ADRankTwoTensor S_bar =
      2 * _mu1 * I2 + 3 * _mu2 * std::sqrt(C_bar_cof.trace()) *
                          (C_bar_cof * C_bar_inv.trace() - C_bar_cof * C_bar_inv);

  ADRankTwoTensor CS_bar = C * S_bar;

  /// isochoric stress
  ADRankTwoTensor S_isc = Jdev * S_bar - C_bar_inv * CS_bar.trace() / 3;

  // Project the right Cauchy strain onto tissue orientation
  ADRankTwoTensor CM1 = C * M1;
  ADRankTwoTensor CM2 = C * M2;
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
  ADRankTwoTensor S = S_isc + S_ti_1 + S_ti_2;

  return S;
}
