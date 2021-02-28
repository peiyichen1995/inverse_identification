#pragma once

#include <Eigen/Dense>
// #include <petsctao.h>

#include "metaphysicl/dualnumberarray.h"

// number of variables in the objective function
#define NVARS 8

typedef double Real;
typedef MetaPhysicL::DualNumber<double, MetaPhysicL::NumberArray<NVARS, double>>
    FirstDerivativeReal;
typedef MetaPhysicL::DualNumber<FirstDerivativeReal,
                                MetaPhysicL::NumberArray<NVARS, FirstDerivativeReal>>
    ADReal;
typedef Eigen::Matrix<Real, 3, 3> RankTwoTensor;
typedef Eigen::Matrix<Real, 3, 1> RankOneTensor;
typedef Eigen::Matrix<ADReal, 3, 3> ADRankTwoTensor;
typedef Eigen::Matrix<ADReal, 3, 1> ADRankOneTensor;
typedef Eigen::Matrix<Real, Eigen::Dynamic, 1> Gradient;
typedef Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic> Hessian;
typedef Eigen::Matrix<Real, Eigen::Dynamic, 1> Vector;
typedef Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic> Matrix;
