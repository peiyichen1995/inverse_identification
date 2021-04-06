#pragma once

#include "Object.hpp"
#include "Coupleable.hpp"

class UserObject : public Object, public Coupleable
{
public:
  UserObject(Problem * problem, hit::Node * params);
  virtual ~UserObject() {}

protected:
private:
};
