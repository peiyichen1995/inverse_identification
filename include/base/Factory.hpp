#pragma once

#include "hit.h"

class Problem;
class Object;

template <typename T>
Object *
createT(Problem * problem, hit::Node * params)
{
  return new T(problem, params);
}

class Factory
{
public:
  typedef std::map<std::string, Object * (*)(Problem *, hit::Node *)> map_type;

  static Object * createObject(const std::string & type, Problem * problem, hit::Node * params)
  {
    map_type::iterator it = getMap()->find(type);
    if (it == getMap()->end())
      return nullptr;
    return it->second(problem, params);
  }

protected:
  static map_type * getMap()
  {
    if (!_map)
      _map = new map_type;
    return _map;
  }

private:
  static map_type * _map;
};

template <typename T>
class Registry : Factory
{
public:
  Registry(const std::string & type) { getMap()->emplace(type, &createT<T>); }
};

#define registerObjectHpp(NAME) static Registry<NAME> reg

#define registerObjectCpp(NAME) Registry<NAME> NAME::reg(#NAME)
