#include "TissueUniaxialTestData.hpp"

registerObjectCpp(TissueUniaxialTestData);

TissueUniaxialTestData::TissueUniaxialTestData(Problem * problem, hit::Node * params)
  : UserObject(problem, params),
    _file_name(params->param<std::string>("file")),
    _angle(params->param<Real>("angle") / 180 * M_PI),
    _add_half_pi(params->paramOptional<bool>("add_half_pi", false))
{
  read();
}

void
TissueUniaxialTestData::read()
{
  std::ifstream infile(_file_name);
  std::string line;
  while (std::getline(infile, line))
  {
    std::istringstream iss(line);
    Real stretch, stress;
    if (!(iss >> stretch >> stress))
      break;
    _stretch.push_back(stretch);
    _stress.push_back(stress);
  }
  infile.close();
}
