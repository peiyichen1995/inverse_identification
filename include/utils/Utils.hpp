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
}
