#include "TissueUniaxialTestDataMismatch.hpp"
#include <algorithm>
#include <fstream>

registerObjectCpp(TissueUniaxialTestDataMismatch);

TissueUniaxialTestDataMismatch::TissueUniaxialTestDataMismatch(Problem * problem,
                                                               hit::Node * params)
  : Objective(problem, params),
    _hyperelastic_model(
        coupledUserObject<HyperelasticModel>(params->param<UserObjectName>("hyperelastic_model"))),
    _lambda_2(coupledValue(params->param<VariableName>("lambda_2"))),
    _lambda_2_name(params->param<VariableName>("lambda_2")),
    _postprocess(params->paramOptional<bool>("postprocess", false)),
    _file_names(
        params->paramOptional<std::vector<std::string>>("file_names", std::vector<std::string>()))
{
  const auto data_names = params->param<std::vector<UserObjectName>>("test_data");
  for (const auto data_name : data_names)
    _datas.push_back(coupledUserObject<TissueUniaxialTestData>(data_name));
}

ADReal
TissueUniaxialTestDataMismatch::uniaxialStress(Real lambda_1, Real angle)
{
  // Solve for lambda2 such that S(2,2) = 0
  Real lambda_2_initial = raw_value(_lambda_2) - 0.05;

  ADRankTwoTensor F;
  F(0, 0) = lambda_1;
  F(1, 1) = _lambda_2;
  F(2, 2) = 1 / lambda_1 / _lambda_2;
  ADRankTwoTensor S = _hyperelastic_model->PK2(F, &angle);
  ADReal f = S(2, 2) * S(2, 2) / _lambda_2;
  Real residual = _problem->firstDerivative(f, _lambda_2_name);
  Real residual_norm = std::abs(residual);

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
      _problem->setVariableValue(_lambda_2_name, lambda_2_initial);
      return NAN;
    }
    Real jacobian = _problem->secondDerivative(f, _lambda_2_name, _lambda_2_name);
    Real delta = -residual / jacobian;
    _problem->setVariableValue(_lambda_2_name, raw_value(_lambda_2) + delta);
    F(0, 0) = lambda_1;
    F(1, 1) = _lambda_2;
    F(2, 2) = 1 / lambda_1 / _lambda_2;
    S = _hyperelastic_model->PK2(F, &angle);
    f = S(2, 2) * S(2, 2) / _lambda_2;
    residual = _problem->firstDerivative(f, _lambda_2_name);
    residual_norm = std::abs(residual);
    its++;
  }

  return lambda_1 * lambda_1 * S(0, 0);
}

ADReal
TissueUniaxialTestDataMismatch::error(unsigned int data_i)
{
  const std::vector<Real> & lambda_1 = _datas[data_i]->stretch();
  const std::vector<Real> & stress_exp = _datas[data_i]->stress();
  std::vector<Real> stress_model;
  Real sigma_exp_max = *std::max_element(stress_exp.begin(), stress_exp.end());

  ADReal f = 0;
  _problem->setVariableValue(_lambda_2_name, 1);
  for (unsigned int i = 0; i < lambda_1.size(); i++)
  {
    if (i > 0)
    {
      Real lambda_1_old = lambda_1[i - 1];
      Real lambda_2_old = raw_value(_lambda_2);
      _problem->setVariableValue(_lambda_2_name, lambda_2_old / lambda_1[i] * lambda_1_old);
    }
    ADReal stress = uniaxialStress(lambda_1[i], _datas[data_i]->angle());
    stress_model.push_back(raw_value(stress));
    f += std::pow((stress_exp[i] - stress) / sigma_exp_max, 2);
  }

  if (_postprocess)
  {
    write(_file_names[data_i], lambda_1, stress_model);
    return 0;
  }

  return f;
}

ADReal
TissueUniaxialTestDataMismatch::value()
{
  ADReal MSE = 0;
  unsigned int N = 0;
  for (unsigned int i = 0; i < _datas.size(); i++)
  {
    MSE += error(i);
    N += _datas[i]->stretch().size();
  }
  return std::sqrt(MSE / N);
}

void
TissueUniaxialTestDataMismatch::write(const std::string file_name,
                                      const std::vector<Real> & x,
                                      const std::vector<Real> & y) const
{
  std::ofstream out;
  out.open(file_name);

  for (unsigned int i = 0; i < x.size(); i++)
    out << x[i] << ", " << y[i] << std::endl;

  out.close();
}
