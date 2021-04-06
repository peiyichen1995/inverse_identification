#pragma once

#include <Eigen/Dense>

#include <map>

#include "metaphysicl/dualdynamicsparsenumberarray.h"

typedef double Real;
typedef MetaPhysicL::DualNumber<double, MetaPhysicL::DynamicSparseNumberArray<Real, unsigned int>>
    FirstDerivativeReal;
typedef MetaPhysicL::DualNumber<
    FirstDerivativeReal,
    MetaPhysicL::DynamicSparseNumberArray<FirstDerivativeReal, unsigned int>>
    ADReal;

typedef Eigen::Matrix<Real, 3, 3> RankTwoTensor;
typedef Eigen::Matrix<Real, 3, 1> RankOneTensor;

typedef Eigen::Matrix<ADReal, 3, 3> ADRankTwoTensor;
typedef Eigen::Matrix<ADReal, 3, 1> ADRankOneTensor;

typedef Eigen::Matrix<Real, Eigen::Dynamic, 1> Vector;
typedef Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic> Matrix;

typedef int DofId;
typedef std::string VariableName;
typedef std::string UserObjectName;
typedef std::map<const VariableName, const DofId> DofMap;
