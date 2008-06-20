#ifndef EVENT_H
#define EVENT_H

#include <string>

struct Event
{
  Event(const std::string& inType) : type(inType) {}
  virtual ~Event() {}

  std::string type;
};

#endif