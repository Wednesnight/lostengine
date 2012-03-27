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

#include "lost/profiler/Blackbox.h"
#include "lost/common/Logger.h"
#include "thread_specific_ptr.h"

namespace lost
{
namespace profiler
{
  tthread::thread_specific_ptr<Blackbox> blackbox_instance;

  Blackbox::Blackbox()
  {
//    DOUT("construct");
  }

  Blackbox::~Blackbox()
  {
//    DOUT("destruct");
  }

  Blackbox::Value& Blackbox::value(const string& inKey)
  {
    return _key2value[inKey];
  }

  void Blackbox::inc(const string& inKey)
  {
    value(inKey).value+=1.0f;
  }
  
  void Blackbox::dec(const string& inKey)
  {
    value(inKey).value-=1.0f;
  }

  void Blackbox::log()
  {
    map<string, Blackbox::Value>::iterator pos;
    DOUT("-- BLACKBOX LOG");
    for(pos=_key2value.begin(); pos!=_key2value.end(); ++pos)
    {
      DOUT(pos->first<<" "<<pos->second.unit<<" "<<pos->second.value);
    }
  }

  void Blackbox::snapshot()
  {
    map<string, Blackbox::Value>::iterator pos;
    for(pos=_key2value.begin(); pos!=_key2value.end(); ++pos)
    {
      Value& v = pos->second;
      if(v.clear)
      {
        v.value=0.0f;
      }
    }    
  }
  
  Blackbox* Blackbox::instance()
  {
    if(!blackbox_instance.get())
    {
      blackbox_instance.reset(new Blackbox);
//      DOUT("Building Blackbox");
    }
//    DOUT("GET "<<(uint64_t)blackbox_instance.get()<<" thread id:"<<(uint64_t)tthread::this_thread::get_id().get());
    return blackbox_instance.get();
  }
  
  BBCount::BBCount(const string& inName)
  {
    name = inName;
    BB_INC(name);
  }
  
  BBCount::~BBCount()
  {
    BB_DEC(name);
  }
}
}
