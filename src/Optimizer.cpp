#include "Optimizer.hpp"
#include <algorithm>

Optimizer::Optimizer(std::array<Real, NVARS> values,
                     const std::vector<ADReal> lambda1s,
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

void
Optimizer::identifyConstraints(Gradient LM,
                               Gradient step,
                               std::vector<unsigned int> & A,
                               std::vector<unsigned int> & I)
{
  Real c = 1e3;
  Gradient indicator = LM - c * step;
  std::cout << indicator << std::endl;

  A.clear();
  I.clear();
  for (unsigned int i = 0; i < LM.rows(); i++)
    if (indicator[i] > 0)
      A.push_back(i);
    else
      I.push_back(i);

  std::cout << "Inactive set: ";
  for (auto i : I)
    std::cout << i << " ";
  std::cout << std::endl;
}

void
Optimizer::optimizeNonnegative(std::vector<unsigned int> var_indices)
{
  // Active set and Inactive set
  std::vector<unsigned int> A, I;

  Gradient step(var_indices.size());
  ADReal f = objective();
  Gradient g = getGradient(f, var_indices);
  Hessian H = getHessian(f, var_indices);
  Gradient LM = -g - H * step;
  identifyConstraints(LM, step, A, I);
  Gradient gr = getGradientOnInactiveSet(g, I);
  Hessian Hr = getHessianOnInactiveSet(H, I);

  Real residual = gr.norm();

  Real ATOL = 1e-10;
  Real RTOL = 1e-8;
  Real residual0 = residual;
  std::cout << "=============================================\n";
  std::cout << "residual = " << residual << std::endl;
  printVariables(var_indices);
  while (residual > ATOL && residual > RTOL * residual0)
  {
    step = -Hr.partialPivLu().solve(gr);
    takeStep(step, var_indices, I);

    f = objective();
    g = getGradient(f, var_indices);
    H = getHessian(f, var_indices);
    LM = -g - H * step;
    identifyConstraints(LM, step, A, I);
    gr = getGradientOnInactiveSet(g, I);
    Hr = getHessianOnInactiveSet(H, I);

    residual = gr.norm();
    std::cout << "=============================================\n";
    std::cout << "residual = " << residual << std::endl;
    printVariables(var_indices);
  }
}

void
Optimizer::optimize(std::vector<unsigned int> var_indices)
{
  ADReal f = objective();
  Gradient g = getGradient(f, var_indices);
  Real residual = g.norm();

  Real ATOL = 1e-10;
  Real RTOL = 1e-8;
  Real residual0 = residual;
  std::cout << "=============================================\n";
  std::cout << "residual = " << residual << std::endl;
  printVariables(var_indices);
  while (residual > ATOL && residual > RTOL * residual0)
  {
    Hessian H = getHessian(f, var_indices);
    Gradient step = -H.partialPivLu().solve(g);
    takeStep(step, var_indices);
    ADReal f = objective();
    Gradient g = getGradient(f, var_indices);
    Real residual = g.norm();
    std::cout << "=============================================\n";
    std::cout << "residual = " << residual << std::endl;
    printVariables(var_indices);
  }
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

Gradient
Optimizer::getGradientOnInactiveSet(Gradient g, const std::vector<unsigned int> & I)
{
  Gradient gr(I.size());
  for (unsigned int i = 0; i < I.size(); i++)
    gr[i] = g[I[i]];
  return gr;
}

Hessian
Optimizer::getHessianOnInactiveSet(Hessian H, const std::vector<unsigned int> & I)
{
  Hessian Hr(I.size(), I.size());
  for (unsigned int i = 0; i < I.size(); i++)
    for (unsigned int j = 0; j < I.size(); j++)
      Hr(i, j) = H(I[i], I[j]);
  return Hr;
}

void
Optimizer::takeStep(Gradient step, const std::vector<unsigned int> & var_indices)
{
  for (unsigned int i = 0; i < var_indices.size(); i++)
    _variables[var_indices[i]].value().value() += step(i);
}

void
Optimizer::takeStep(Gradient step,
                    const std::vector<unsigned int> & var_indices,
                    const std::vector<unsigned int> & I)
{
  for (auto i : I)
    _variables[var_indices[i]].value().value() += step(i);
}

void
Optimizer::printVariables(std::vector<unsigned int> var_indices)
{
  std::cout << "current solution = [";
  for (auto i : var_indices)
    std::cout << _variables[i].value().value() << " ";
  std::cout << "]\n";
}

ADReal
Optimizer::objective()
{
  std::ofstream sigma_file("sigma.csv");

  ADReal sigma_exp_max = _sigma_exp[0];
  for (auto s : _sigma_exp)
    if (s > sigma_exp_max)
      sigma_exp_max = s;

  ADReal f = 0;
  for (unsigned int i = 0; i < _lambda1s.size(); i++)
  {
    ADReal sigma = stress(_lambda1s[i]);
    sigma_file << sigma.value().value() << std::endl;
    f += std::pow((_sigma_exp[i] - sigma) / sigma_exp_max, 2.0);
  }
  f = std::sqrt(f / _lambda1s.size());

  sigma_file.close();

  return f;
}

ADReal
Optimizer::stress(ADReal lambda1)
{
  // Solve for lambda 2 such that S(2,2) = 0
  ADReal & lambda2 = _variables[4];

  // Initial guess for lambda2
  lambda2.value().value() = 1;
  ADRankTwoTensor S = PK2(lambda1);
  Real S22 = S(2, 2).value().value();

  Real ATOL = 1e-10;
  Real RTOL = 1e-8;
  int max_its = 20;
  Real S22_0 = S22;
  int its = 0;
  while (std::abs(S22) > ATOL && std::abs(S22) > RTOL * std::abs(S22_0))
  {
    if (its > max_its)
    {
      std::cout << "Too many iterations in stress()" << std::endl;
      exit(1);
    }
    Real dS33_dlambda2 = S(2, 2).value().derivatives()[4];
    Real delta = -S22 / dS33_dlambda2;
    lambda2.value().value() = lambda2.value().value() + delta;
    S = PK2(lambda1);
    S22 = S(2, 2).value().value();
    its++;
  }

  return lambda1 * lambda1 * S(0, 0);
}

ADRankTwoTensor
Optimizer::PK2(ADReal lambda1)
{
  ADReal mu1 = _variables[0];
  ADReal mu2 = _variables[1];
  ADReal mu4 = _variables[2];
  ADReal beta4 = _variables[3];
  ADReal lambda2 = _variables[4];

  ADRankTwoTensor I2, M1, M2;
  ADRankTwoTensor F, C, C_bar, C_bar_inv, C_bar_cof, CM1, CM2;
  ADRankTwoTensor S_bar, CS_bar, S_isc, S_vol, S_ti_1, S_ti_2, S;

  ADReal angle = 22.2 * M_PI / 180.0;
  ADRankOneTensor a1(std::cos(angle), -std::sin(angle), 0.0);
  ADRankOneTensor a2(std::cos(angle), std::sin(angle), 0.0);

  I2.setIdentity();
  M1 = cross(a1, a1);
  M2 = cross(a2, a2);

  F(0, 0) = lambda1;
  F(1, 1) = lambda2;
  F(2, 2) = 1.0 / lambda1 / lambda2;
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

  // Due to the incompressibility, J = 1, S_vol = 0
  // ADReal p = mu3 * beta3 * (std::pow(J, beta3 - 1.0) - std::pow(J, -beta3 - 1.0));
  // S_vol = p * J * C.inverse();

  CM1 = mult(C, M1);
  CM2 = mult(C, M2);
  ADReal e1pos = CM1.trace() - 1.0;
  ADReal e2pos = CM2.trace() - 1.0;

  S_ti_1 = 4.0 * mu4 * e1pos * std::exp(beta4 * e1pos * e1pos) * M1;
  S_ti_2 = 4.0 * mu4 * e2pos * std::exp(beta4 * e2pos * e2pos) * M2;

  S = S_isc;
  if (e1pos > 0.0)
    S += S_ti_1;
  if (e2pos > 0.0)
    S += S_ti_2;

  return S;
}
