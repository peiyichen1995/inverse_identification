#pragma once

#include "Types.hpp"
#include "hit.h"

namespace Utils
{
const std::string dline = "===================================================";
const std::string sline = "---------------------------------------------------";
const std::string tab = "    ";

inline const std::string
indent(unsigned int n)
{
  std::string ret = "";
  for (unsigned int i = 0; i < n; i++)
    ret += tab;
  return ret;
}

inline RankTwoTensor
raw_value(const ADRankTwoTensor & a)
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
