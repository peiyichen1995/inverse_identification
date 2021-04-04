#pragma once

#include "Object.hpp"
#include "Coupleable.hpp"

template <typename T>
class Property;

template <typename T>
class Property : public Object, public Coupleable
{
public:
  Property(Problem * problem, hit::Node * params);

protected:
private:
  T _v;

  template <typename T2>
  friend std::ostream & operator<<(std::ostream & os, const Property<T2> & v);
};

template <typename T>
std::ostream & operator<<(std::ostream & os, const Property<T> & v);
