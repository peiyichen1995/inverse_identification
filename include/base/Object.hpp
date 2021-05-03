#pragma once

#include <iostream>
#include "Types.hpp"
#include "Factory.hpp"
#include "Utils.hpp"

class Problem;

class Object
{
public:
  Object(Problem * problem, hit::Node * params);
  virtual ~Object() {}

  const std::string name() const { return _name; }
  const std::string type() const { return _type; }
  Problem * problem() { return _problem; }

  virtual void print(std::ostream & os = std::cout) const;
  friend std::ostream & operator<<(std::ostream & os, const Object & o);

protected:
  Problem * _problem;
  unsigned int & _indent;

private:
  const std::string _name;
  const std::string _type;
};

std::ostream & operator<<(std::ostream & os, const Object & o);
