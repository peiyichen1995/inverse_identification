#pragma once

#include <Eigen/Dense>

#include "metaphysicl/dualnumberarray.h"

// number of variables in the objective function
#define NVARS 5

typedef double Real;
typedef MetaPhysicL::DualNumber<double, MetaPhysicL::NumberArray<NVARS, double>>
    FirstDerivativeReal;
typedef MetaPhysicL::DualNumber<FirstDerivativeReal,
                                MetaPhysicL::NumberArray<NVARS, FirstDerivativeReal>>
    ADReal;
typedef Eigen::Matrix<ADReal, 3, 3> ADRankTwoTensor;
typedef Eigen::Matrix<ADReal, 3, 1> ADRankOneTensor;
typedef Eigen::VectorXd Gradient;
typedef Eigen::MatrixXd Hessian;
