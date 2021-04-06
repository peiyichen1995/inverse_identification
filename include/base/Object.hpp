#pragma once

#include <iostream>
#include <iomanip>
#include "Types.hpp"
#include "Factory.hpp"
#include "hit.h"

#define OBJECT_NAME_MAX_LENGTH 20

class Object
{
public:
  Object(hit::Node * params);
  virtual ~Object() {}

  const std::string name() const { return _name; }
  const std::string type() const { return _type; }

protected:
private:
  const std::string _name;
  const std::string _type;
};
