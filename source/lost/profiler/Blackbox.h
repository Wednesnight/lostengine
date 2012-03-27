/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
