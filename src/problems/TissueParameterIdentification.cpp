#include "TissueParameterIdentification.hpp"

#include <fstream>
#include <sstream>
#include <iomanip>

void
TissueParameterIdentification::addExperimentalData(const char * file_name)
{
  ExperimentalData data;
  std::ifstream infile(file_name);
  std::string line;
  while (std::getline(infile, line))
  {
    std::istringstream iss(line);
    Real lambda, sigma, angle;
    if (!(iss >> lambda >> sigma >> angle))
      break;
    data.push_back({lambda, sigma, angle});
  }
  infile.close();
  _data.push_back(data);
}

void
TissueParameterIdentification::printExperimentalData(std::ostream & os) const
{
  for (unsigned int i = 0; i < _data.size(); i++)
  {
    os << "=======================================" << std::endl;
    os << "Experimental data " << i << std::endl;
    os << "---------------------------------------" << std::endl;
    os << std::setw(10) << "lambda" << std::setw(10) << "sigma" << std::setw(10) << "angle"
       << std::endl;
    os << "---------------------------------------" << std::endl;
    for (auto row : _data[i])
      os << std::setw(10) << std::get<0>(row) << std::setw(10) << std::get<1>(row) << std::setw(10)
         << std::get<2>(row) << std::endl;
    os << "=======================================" << std::endl;
  }
}

std::pair<Real, Vector>
TissueParameterIdentification::computeObjectiveAndGradient(const Vector & x)
{
  setVariableValue(x);

  bool bad_guess = false;

  ADReal f = 0;
  for (auto data : _data)
  {
    ADReal sigma_exp_max = std::get<1>(data[0]);
    for (auto row : data)
      if (std::get<1>(row) > sigma_exp_max)
        sigma_exp_max = std::get<1>(row);

    Real lambda1, lambda2, angle;
    for (unsigned int i = 0; i < data.size(); i++)
    {
      lambda1 = std::get<0>(data[i]);
      angle = std::get<2>(data[i]);
      if (i == 0)
        lambda2 = 1 / std::sqrt(lambda1);
      if (i > 0)
      {
        Real lambda1_old = std::get<0>(data[i - 1]);
        lambda2 = raw_value(getVariableValue("lambda_2")) / lambda1 * lambda1_old;
      }
      setVariableValue("lambda_1", lambda1);
      setVariableValue("lambda_2", lambda2);
      setVariableValue("tissue_angle", angle);

      printVariableValues();

      ADReal sigma_exp = std::get<1>(data[i]);
      ADReal sigma = computeStress();
      if (std::isnan(sigma.value().value()) || std::isinf(sigma.value().value()))
      {
        bad_guess = true;
        sigma = 0;
      }
      f += std::pow((sigma_exp - sigma) / sigma_exp_max, 2) / data.size();
    }
  }
  f = std::sqrt(f);
  if (bad_guess)
    std::cout << "Had a bad guess because a stress is inf or nan, retrying. Wish me good luck..."
              << std::endl;

  return {f.value().value(), getGradient(f)};
}

ADReal
TissueParameterIdentification::computeStress()
{
  ADReal lambda1 = getVariableValue("lambda_1");
  ADReal lambda2 = getVariableValue("lambda_2");

  // Solve for lambda2 such that S(2,2) = 0
  ADRankTwoTensor F;
  F(0, 0) = lambda1;
  F(1, 1) = lambda2;
  F(2, 2) = 1 / lambda1 / lambda2;
  ADRankTwoTensor S = _model->PK2(F);
  ADReal f = S(2, 2) / lambda2 / lambda2;
  Vector residual = getGradient(f, getVariableIndices({"lambda_2"}));
  Real residual_norm = residual.norm();
  std::cout << "=============================================\n";
  std::cout << "residual = " << residual_norm << ", lambda2 = " << raw_value(lambda2) << std::endl;

  Real ATOL = 1e-10;
  Real RTOL = 1e-8;
  int max_its = 50;
  ADReal lambda2_initial = lambda2;
  Real residual_norm_0 = residual_norm;
  int its = 0;
  while (residual_norm > ATOL && residual_norm > RTOL * residual_norm_0)
  {
    if (its > max_its)
    {
      std::cout << "Too many iterations in stress()" << std::endl;
      lambda2 = lambda2_initial;
      return NAN;
    }
    Matrix jacobian = getHessian(f, getVariableIndices({"lambda_2"}));
    Vector delta = -jacobian.partialPivLu().solve(residual);
    setVariableValue("lambda_2", raw_value(lambda2) + delta[0]);
    lambda2 = getVariableValue("lambda_2");
    F(1, 1) = lambda2;
    F(2, 2) = 1 / lambda1 / lambda2;
    S = _model->PK2(F);
    f = S(2, 2) / lambda2 / lambda2;
    residual = getGradient(f, getVariableIndices({"lambda_2"}));
    residual_norm = residual.norm();
    std::cout << "residual = " << residual_norm << ", lambda2 = " << raw_value(lambda2)
              << std::endl;
    its++;
  }

  return lambda1 * lambda1 * S(0, 0);
}
