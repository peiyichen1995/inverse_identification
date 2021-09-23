#include "TissueUniaxialTestDataMismatchWithAngle.hpp"
#include <algorithm>
#include <fstream>

registerObjectCpp(TissueUniaxialTestDataMismatchWithAngle);

TissueUniaxialTestDataMismatchWithAngle::TissueUniaxialTestDataMismatchWithAngle(Problem * problem,
                                                               hit::Node * params)
  : TissueUniaxialTestDataMismatch(problem, params),
  _angle(coupledValue(params->param<VariableName>("angle")))
{
}


ADReal
TissueUniaxialTestDataMismatchWithAngle::error(unsigned int data_i)
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
    ADReal stress;
    if (_datas[data_i]->addHalfPi())
        stress = uniaxialStress(lambda_1[i], _angle/180*M_PI + M_PI/2);
    else
      stress = uniaxialStress(lambda_1[i], _angle/180*M_PI);


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
