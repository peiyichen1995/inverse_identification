#include "Problem.hpp"

void
Problem::init()
{
  unsigned int nvars = _variables.size() + _aux_variables.size();
  _values.resize(nvars);
  for (unsigned int i = 0; i < nvars; i++)
  {
    _values[i].value() = 0;
    _values[i].value().derivatives()[i] = 1;
    for (unsigned int j = 0; j < nvars; j++)
      _values[i].derivatives()[j] = i == j ? 1 : 0;
  }
}

void
Problem::printInfo(std::ostream & os) const
{
  os << "Variables: " << std::endl;
  for (auto v : _variables)
    os << "  " << getVariableIndex(v.first) << ": " << v.first << std::endl;
  os << "Auxiliary Variables: " << std::endl;
  for (auto v : _aux_variables)
    os << "  " << getVariableIndex(v.first) << ": " << v.first << std::endl;
}

void
Problem::printVariableValues(std::ostream & os) const
{
  os << "Variable values: " << std::endl;
  for (auto v : _variables)
    os << "  " << v.first << " = " << raw_value(getVariableValue(v.first)) << std::endl;
  os << "Auxiliary variable values: " << std::endl;
  for (auto v : _aux_variables)
    os << "  " << v.first << " = " << raw_value(getVariableValue(v.first)) << std::endl;
}

void
Problem::addVariable(std::string var_name)
{
  if (_variables.count(var_name) == 0)
    _variables.emplace(var_name, _variables.size());
}

void
Problem::addVariables(std::vector<std::string> var_names)
{
  for (auto v : var_names)
    addVariable(v);
}

void
Problem::addAuxVariable(std::string var_name)
{
  if (_aux_variables.count(var_name) == 0)
    _aux_variables.emplace(var_name, _aux_variables.size());
}

void
Problem::addAuxVariables(std::vector<std::string> var_names)
{
  for (auto v : var_names)
    addAuxVariable(v);
}

unsigned int
Problem::getVariableIndex(std::string var_name) const
{
  auto vit = _variables.find(var_name);
  if (vit != _variables.end())
    return vit->second;
  else
  {
    auto avit = _aux_variables.find(var_name);
    if (avit != _aux_variables.end())
      return _variables.size() + avit->second;
    else
      std::cerr << "Error: variable " << var_name << " not found." << std::endl;
  }
  return 0;
}

std::vector<unsigned int>
Problem::getVariableIndices(std::vector<std::string> var_names) const
{
  std::vector<unsigned int> indices;
  for (auto v : var_names)
    indices.push_back(getVariableIndex(v));
  return indices;
}

const ADReal &
Problem::getVariableValue(std::string var_name) const
{
  unsigned int index = getVariableIndex(var_name);
  return _values[index];
}

void
Problem::setVariableValue(unsigned int i, Real v)
{
  _values[i].value().value() = v;
}

void
Problem::setVariableValue(std::string var_name, Real v)
{
  _values[getVariableIndex(var_name)].value().value() = v;
}

void
Problem::setVariableValue(const Vector & x)
{
  for (unsigned int i = 0; i < _variables.size(); i++)
    setVariableValue(i, x[i]);
}
