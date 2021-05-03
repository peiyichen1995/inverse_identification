#pragma once

#include "Object.hpp"

class Variable : public Object
{
public:
  Variable(Problem * problem, hit::Node * params);

  const ADReal & value() const { return _value; }
  ADReal & set() { return _value; }

  Real lowerBound() const { return _lower_bound; }
  Real upperBound() const { return _upper_bound; }

  virtual void print(std::ostream & os = std::cout) const override;

protected:
private:
  ADReal _value;

  const Real _lower_bound;
  const Real _upper_bound;
};
