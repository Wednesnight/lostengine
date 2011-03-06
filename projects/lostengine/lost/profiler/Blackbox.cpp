#include "lost/profiler/Blackbox.h"
#include "lost/common/Logger.h"
#include <boost/thread.hpp>

namespace lost
{
namespace profiler
{
  boost::thread_specific_ptr<Blackbox> blackbox_instance;

  Blackbox::Blackbox()
  {
    DOUT("construct");
  }

  Blackbox::~Blackbox()
  {
    DOUT("destruct");
  }

  Blackbox::Value& Blackbox::value(const std::string& inKey)
  {
    return _key2value[inKey];
  }

  void Blackbox::inc(const std::string& inKey)
  {
    value(inKey).value+=1.0f;
  }
  
  void Blackbox::dec(const std::string& inKey)
  {
    value(inKey).value-=1.0f;
  }

  void Blackbox::log()
  {
    std::map<std::string, Blackbox::Value>::iterator pos;
    DOUT("-- BLACKBOX LOG");
    for(pos=_key2value.begin(); pos!=_key2value.end(); ++pos)
    {
      DOUT(pos->first<<" "<<pos->second.unit<<" "<<pos->second.value);
    }
  }

  void Blackbox::snapshot()
  {
    std::map<std::string, Blackbox::Value>::iterator pos;
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
//    DOUT("GET "<<(uint64_t)blackbox_instance.get()<<" thread id:"<<boost::this_thread::get_id());
    return blackbox_instance.get();
  }
  
  BBCount::BBCount(const std::string& inName)
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
