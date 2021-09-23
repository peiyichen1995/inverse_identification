#pragma once

#include "UserObject.hpp"

class TissueUniaxialTestData : public UserObject
{
public:
  TissueUniaxialTestData(Problem * problem, hit::Node * params);

  void read();
  const std::vector<Real> & stretch() const { return _stretch; }
  const std::vector<Real> & stress() const { return _stress; }
  Real angle() const { return _angle; }
  bool addHalfPi() const { return _add_half_pi; }

protected:
private:
  registerObjectHpp(TissueUniaxialTestData);

  const std::string _file_name;
  std::vector<Real> _stretch;
  std::vector<Real> _stress;
  const Real _angle;
  const bool _add_half_pi;
};
