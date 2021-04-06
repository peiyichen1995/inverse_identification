#pragma once

#include "Object.hpp"

class Variable : public Object
{
public:
  Variable(hit::Node * params);

  const ADReal & value() const { return _value; }
  ADReal & set() { return _value; }

  Real lowerBound() const { return _lower_bound; }
  Real upperBound() const { return _upper_bound; }

protected:
private:
  ADReal _value;

  const Real _lower_bound;
  const Real _upper_bound;

  friend std::ostream & operator<<(std::ostream & os, const Variable & v);
};

std::ostream & operator<<(std::ostream & os, const Variable & v);
