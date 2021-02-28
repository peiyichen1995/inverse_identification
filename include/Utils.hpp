#pragma once

#include "Types.hpp"

namespace Utils
{
inline ADRankTwoTensor
mult(const ADRankTwoTensor a, const ADRankTwoTensor b)
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

inline ADRankTwoTensor
cross(const ADRankOneTensor a, const ADRankOneTensor b)
{
  ADRankTwoTensor r;
  for (unsigned int i = 0; i < 3; i++)
    for (unsigned int j = 0; j < 3; j++)
      r(i, j) = a[i] * b[j];
  return r;
}

inline RankTwoTensor
raw_value(const ADRankTwoTensor a)
{
  RankTwoTensor a_raw;
  a_raw(0, 0) = raw_value(a(0, 0));
  a_raw(1, 0) = raw_value(a(1, 0));
  a_raw(2, 0) = raw_value(a(2, 0));
  a_raw(0, 1) = raw_value(a(0, 1));
  a_raw(1, 1) = raw_value(a(1, 1));
  a_raw(2, 1) = raw_value(a(2, 1));
  a_raw(0, 2) = raw_value(a(0, 2));
  a_raw(1, 2) = raw_value(a(1, 2));
  a_raw(2, 2) = raw_value(a(2, 2));
  return a_raw;
}
}
