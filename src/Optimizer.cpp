#include "Optimizer.hpp"
#include <algorithm>

Optimizer::Optimizer(std::array<Real, NVARS> values,
                     const std::vector<std::pair<ADReal, ADReal>> lambda1s,
                     const std::vector<ADReal> sigma_exp)
  : _lambda1s(lambda1s), _sigma_exp(sigma_exp)
{
  for (unsigned int i = 0; i < NVARS; i++)
  {
    _variables[i].value() = values[i];
    _variables[i].value().derivatives()[i] = 1;
    for (unsigned int j = 0; j < NVARS; j++)
      _variables[i].derivatives()[j] = i == j ? 1 : 0;
  }
}

ADRankTwoTensor
Optimizer::mult(const ADRankTwoTensor & a, const ADRankTwoTensor & b)
{
  ADRankTwoTensor r;
  for (unsigned int i = 0; i < 3; i++)
    for (unsigned int j = 0; j < 3; j++)
    {
      r(i, j) = 0;
      for (unsigned int k = 0; k < 3; k++)
        r(i, j) += a(i, k) * b(k, j);
    }
  return r;
}

ADRankTwoTensor
Optimizer::cross(const ADRankOneTensor & a, const ADRankOneTensor & b)
{
  ADRankTwoTensor r;
  for (unsigned int i = 0; i < 3; i++)
    for (unsigned int j = 0; j < 3; j++)
      r(i, j) = a[i] * b[j];
  return r;
}

Gradient
Optimizer::getGradient(ADReal f, std::vector<unsigned int> var_indices)
{
  Gradient g(var_indices.size());
  for (unsigned int i = 0; i < var_indices.size(); i++)
    g[i] = f.value().derivatives()[var_indices[i]];
  return g;
}

Hessian
Optimizer::getHessian(ADReal f, std::vector<unsigned int> var_indices)
{
  Hessian H(var_indices.size(), var_indices.size());
  for (unsigned int i = 0; i < var_indices.size(); i++)
    for (unsigned int j = 0; j < var_indices.size(); j++)
      H(i, j) = f.derivatives()[var_indices[i]].derivatives()[var_indices[j]];
  return H;
}

ADReal
Optimizer::objective()
{
  ADReal sigma_exp_max = _sigma_exp[0];
  for (auto s : _sigma_exp)
    if (s > sigma_exp_max)
      sigma_exp_max = s;

  ADReal f = 0;
  _variables[7].value().value() = 1;
  // _variables[8].value().value() = 1;
  bool bad_guess = false;
  for (unsigned int i = 0; i < _lambda1s.size(); i++)
  {
    if (i > 0)
    {
      if (_lambda1s[i].first < _lambda1s[i - 1].first)
        _variables[7].value().value() = 1;
      else
      {
        Real lambda1_old = _lambda1s[i - 1].first.value().value();
        Real lambda2_old = _variables[7].value().value();
        // Real lambda3_old = _variables[8].value().value();
        // Real C33_old = 1.0 / lambda1_old / lambda2_old;
        // Real C33_old = 1.0 / lambda1_old / lambda1_old / lambda2_old / lambda2_old;
        // Real C33 = C33_old / _lambda1s[i].first.value().value() / lambda1_old;
        _variables[7].value().value() =
            lambda2_old / _lambda1s[i].first.value().value() * lambda1_old;
        // _variables[8].value().value() =
        //     lambda3_old * _lambda1s[i].first.value().value() / lambda1_old;
      }
    }
    ADReal sigma = stress(_lambda1s[i].first, _lambda1s[i].second);
    if (std::isnan(sigma.value().value()) || std::isinf(sigma.value().value()))
    {
      bad_guess = true;
      sigma = 0;
    }
    f += std::pow((_sigma_exp[i] - sigma) / sigma_exp_max, 2.0);
  }
  f = std::sqrt(f / _lambda1s.size());

  if (bad_guess)
    std::cout << "Had a bad guess because a stress is inf or nan, retrying. Wish me good luck..."
              << std::endl;

  return f;
}

ADReal
Optimizer::objective(const PetscReal * x)
{
  _variables[0].value().value() = x[0];
  _variables[1].value().value() = x[1];
  _variables[2].value().value() = x[2];
  _variables[3].value().value() = x[3];
  _variables[4].value().value() = x[4];
  _variables[5].value().value() = x[5];
  _variables[6].value().value() = x[6];
  return objective();
}

ADReal
Optimizer::stress(ADReal lambda1, ADReal angle)
{
  // Solve for lambda2 such that S(2,2) = 0
  ADReal & lambda2 = _variables[7];
  Real lambda2_initial = lambda2.value().value();
  // ADReal & lambda3 = _variables[8];

  // Initial guess for lambda2
  // lambda2.value().value() = 1.0 / std::sqrt(lambda1.value().value());
  // lambda3.value().value() = 1.0 / std::sqrt(lambda1.value().value());
  ADRankTwoTensor S = PK2(lambda1, angle);
  ADReal f = S(2, 2) / lambda2 / lambda2;
  // ADReal f = S(1, 1) * S(1, 1) / 2.0 + S(2, 2) * S(2, 2) / 2.0;
  // ADReal f = std::abs(S(2, 2));
  Gradient residual = getGradient(f, {7});
  Real residual_norm = residual.norm();
  // std::cout << "=============================================\n";
  // std::cout << "residual = " << residual_norm << ", lambda2 = " << lambda2.value().value()
  //           << std::endl;

  Real ATOL = 1e-10;
  Real RTOL = 1e-8;
  int max_its = 50;
  Real residual_norm_0 = residual_norm;
  int its = 0;
  while (residual_norm > ATOL && residual_norm > RTOL * residual_norm_0)
  {
    if (its > max_its)
    {
      std::cout << "Too many iterations in stress()" << std::endl;
      lambda2.value().value() = lambda2_initial;
      return NAN;
    }
    Hessian jacobian = getHessian(f, {7});
    Gradient delta = -jacobian.partialPivLu().solve(residual);
    lambda2.value().value() += delta[0];
    // lambda3.value().value() += delta[1];
    S = PK2(lambda1, angle);
    f = S(2, 2) / lambda2 / lambda2;
    // f = S(1, 1) * S(1, 1) / 2.0 + S(2, 2) * S(2, 2) / 2.0;
    // f = std::abs(S(2, 2));
    residual = getGradient(f, {7});
    residual_norm = residual.norm();
    // std::cout << "residual = " << residual_norm << ", lambda2 = " << lambda2.value().value()
    //           << std::endl;
    its++;
  }

  return lambda1 * lambda1 * S(0, 0);
}

ADRankTwoTensor
Optimizer::PK2(ADReal lambda1, ADReal angle_degree)
{
  ADReal mu1 = _variables[0];
  ADReal mu2 = _variables[1];
  ADReal mu3 = _variables[2];
  ADReal mu4 = _variables[3];
  ADReal beta3 = _variables[4];
  ADReal beta4 = _variables[5];
  ADReal rho = _variables[6];
  ADReal lambda2 = _variables[7];
  // ADReal lambda3 = _variables[8];

  ADRankTwoTensor I2, M1, M2;
  ADRankTwoTensor F, C, C_bar, C_bar_inv, C_bar_cof, CM1, CM2;
  ADRankTwoTensor S_bar, CS_bar, S_isc, S_vol, S_ti_1, S_ti_2, S;

  ADReal angle = angle_degree * M_PI / 180.0;
  ADRankOneTensor a1(std::cos(angle), -std::sin(angle), 0.0);
  ADRankOneTensor a2(std::cos(angle), std::sin(angle), 0.0);

  I2.setIdentity();
  M1 = cross(a1, a1);
  M2 = cross(a2, a2);

  F(0, 0) = lambda1;
  F(1, 1) = lambda2;
  F(2, 2) = 1 / lambda1 / lambda2;
  // F(2, 2) = lambda3;
  ADReal J = F.determinant();
  ADReal Jdev = std::pow(J, -2.0 / 3.0);

  C = mult(F.transpose(), F);
  C_bar = Jdev * C;
  C_bar_inv = C_bar.inverse();
  C_bar_cof = C_bar.determinant() * C_bar_inv;

  S_bar = 2.0 * mu1 * I2 + 3.0 * mu2 * std::sqrt(C_bar_cof.trace()) *
                               (C_bar_cof * C_bar_inv.trace() - mult(C_bar_cof, C_bar_inv));
  CS_bar = mult(C, S_bar);
  S_isc = Jdev * S_bar - C_bar_inv * CS_bar.trace() / 3.0;

  ADReal p = mu3 * beta3 * (std::pow(J, beta3 - 1.0) - std::pow(J, -beta3 - 1.0));
  S_vol = p * J * C.inverse();

  CM1 = mult(C, M1);
  CM2 = mult(C, M2);
  ADReal e1 = CM1.trace() - 1.0;
  ADReal e2 = CM2.trace() - 1.0;
  ADReal e1pos = (std::abs(e1) + e1) / 2.0;
  ADReal e2pos = (std::abs(e2) + e2) / 2.0;
  // ADReal C_bar_tr = C_bar.trace() - 3.0;
  ADReal C_tr = C.trace() - 3.0;
  // ADReal Cbtrpos = (std::abs(C_bar_tr) + C_bar_tr) / 2.0;
  // ADReal Jvol = J * J - 1.0;
  // ADRankTwoTensor DevCbar = Jdev * I2 - C_bar_inv.trace() * C;

  // S_ti_1 = 4.0 * mu4 * e1pos * std::exp(beta4 * e1pos * e1pos) * M1;
  // S_ti_2 = 4.0 * mu4 * e2pos * std::exp(beta4 * e2pos * e2pos) * M2;

  S_ti_1 = 4.0 * mu4 * ((1.0 - rho) * C_tr * I2 + rho * e1pos * M1) *
           std::exp(beta4 * ((1.0 - rho) * C_tr * C_tr + rho * e1pos * e1pos));
  S_ti_2 = 4.0 * mu4 * ((1.0 - rho) * C_tr * I2 + rho * e2pos * M2) *
           std::exp(beta4 * ((1.0 - rho) * C_tr * C_tr + rho * e2pos * e2pos));
  S = S_ti_1 + S_ti_2;

  return S;
}
