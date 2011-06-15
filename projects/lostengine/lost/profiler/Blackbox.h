#ifndef LOST_PROFILER_BLACKBOX_H
#define LOST_PROFILER_BLACKBOX_H

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
    string unit;
  };  
  
  
  
  Blackbox();
  ~Blackbox();
  static Blackbox* instance(); // returns thread specific instance
  
  Value& value(const string& inKey);
  void inc(const string& inKey);
  void dec(const string& inKey);

  void snapshot();
  void log();

private:  
  map<string, Value> _key2value;  
};

struct BBCount
{
  BBCount(const string& inName);
  ~BBCount();
  
  string name;
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