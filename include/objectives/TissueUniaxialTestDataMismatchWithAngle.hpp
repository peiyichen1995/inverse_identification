#pragma once

#include "TissueUniaxialTestDataMismatch.hpp"

class TissueUniaxialTestDataMismatchWithAngle : public TissueUniaxialTestDataMismatch
{
public:
  TissueUniaxialTestDataMismatchWithAngle(Problem * problem, hit::Node * params);


protected:
  virtual ADReal error(unsigned int i) override;

  const ADReal & _angle;

private:
  registerObjectHpp(TissueUniaxialTestDataMismatchWithAngle);
};
