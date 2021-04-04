#pragma once

#include "Object.hpp"

class Variable : public Object
{
public:
  Variable(hit::Node * params);

  const ADReal & value() const { return _value; }
  ADReal & set() { return _value; }

protected:
private:
  ADReal _value;

  friend std::ostream & operator<<(std::ostream & os, const Variable & v);
};

std::ostream & operator<<(std::ostream & os, const Variable & v);
