#include "Object.hpp"

Object::Object(hit::Node * params)
  : _name(params->path()), _type(params->paramOptional<std::string>("type", "None"))
{
  if (_name.size() > 20)
  {
    std::cout << "Object name: " << _name << " is too long, maximum allowable name length is "
              << OBJECT_NAME_MAX_LENGTH << std::endl;
    exit(1);
  }
}
