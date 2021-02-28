#pragma once

#include <iostream>
#include <vector>
#include <map>

#include "Types.hpp"

class Problem
{
public:
  Problem() {}
  virtual ~Problem() {}

  virtual void init();
  virtual void printInfo(std::ostream & os = std::cout) const;
  void printVariableValues(std::ostream & os = std::cout) const;

  void addVariable(std::string var_name);
  void addVariables(std::vector<std::string> var_names);
  void addAuxVariable(std::string var_name);
  void addAuxVariables(std::vector<std::string> var_names);
  unsigned int getVariableIndex(std::string var_name) const;
  std::vector<unsigned int> getVariableIndices(std::vector<std::string> var_names) const;
  const ADReal & getVariableValue(std::string var_name) const;

  void setVariableValue(unsigned int i, Real v);
  void setVariableValue(std::string var_name, Real v);
  void setVariableValue(const Vector & x);

protected:
  std::map<std::string, unsigned int> _variables;
  std::map<std::string, unsigned int> _aux_variables;

private:
  std::vector<ADReal> _values;
};
