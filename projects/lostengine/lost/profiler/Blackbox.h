#ifndef LOST_PROFILER_BLACKBOX_H
#define LOST_PROFILER_BLACKBOX_H

#include <string>
#include <map>

namespace lost
{
namespace profiler
{
struct Blackbox
{
  struct Value
  {
    Value() {value=0.0f; clear=false;unit="#";}
    float value;
    bool clear;
    std::string unit;
  };  
  
  
  
  Blackbox();
  ~Blackbox();
  static Blackbox* instance(); // returns thread specific instance
  
  Value& value(const std::string& inKey);
  void inc(const std::string& inKey);
  void dec(const std::string& inKey);

  void snapshot();
  void log();

private:  
  std::map<std::string, Value> _key2value;  
};

struct BBCount
{
  BBCount(const std::string& inName);
  ~BBCount();
  
  std::string name;
};
}
}

#define BB_INC(name) lost::profiler::Blackbox::instance()->inc(name);
#define BB_DEC(name) lost::profiler::Blackbox::instance()->dec(name);
#define BB_SET(name,val) lost::profiler::Blackbox::instance()->value(name).value=val;
#define BB_LOG lost::profiler::Blackbox::instance()->log();
#define BB_SNAP lost::profiler::Blackbox::instance()->snapshot();
#define BB_SET_CLEAR(name,flag) lost::profiler::Blackbox::instance()->value(name).clear=flag;
#define BB_SET_UNIT(name,val) lost::profiler::Blackbox::instance()->value(name).unit=val;
#endif