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

inline ADRankTwoTensor
cross(const ADRankOneTensor & a, const ADRankOneTensor & b)
{
  ADRankTwoTensor r;
  for (unsigned int i = 0; i < 3; i++)
    for (unsigned int j = 0; j < 3; j++)
      r(i, j) = a[i] * b[j];
  return r;
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

inline std::string
hitCLIFilter(const std::vector<std::string> & argv)
{
  std::string hit_text;
  bool afterDoubleDash = false;
  for (std::size_t i = 1; i < argv.size(); i++)
  {
    std::string arg(argv[i]);

    // all args after a "--" are hit parameters
    if (arg == "--")
    {
      afterDoubleDash = true;
      continue;
    } // otherwise try to guess if a hit params have started by looking for "=" and "/"
    else if (arg.find("=", 0) != std::string::npos)
      afterDoubleDash = true;

    // skip over args that don't look like or are before hit parameters
    if (!afterDoubleDash)
      continue;
    // skip arguments with no equals sign
    if (arg.find("=", 0) == std::string::npos)
      continue;
    // skip cli flags (i.e. start with dash)
    if (arg.find("-", 0) == 0)
      continue;

    auto pos = arg.find(":", 0);
    if (pos == 0) // trim leading colon
      arg = arg.substr(pos + 1, arg.size() - pos - 1);
    else if (pos != std::string::npos && pos < arg.find("=", 0)) // param is for non-main subapp
      continue;

    hit::check("CLI_ARG", arg);
    hit_text += " " + arg;
    // handle case where bash ate quotes around an empty string after the "="
    if (arg.find("=", 0) == arg.size() - 1)
      hit_text += "''";
  }
  return hit_text;
}
}
