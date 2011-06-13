#include <iostream>
#include "tinythread.h"
#include "lost/event/Event.h"
#include "lost/event/Listener.h"
#include "lost/event/Signal.h"
#include <EASTL/string.h>
#include "lost/common/eastlStreamSupport.h"
#include "lost/common/Logger.h"

#include "lost/application/Runner.h"

using namespace std;

static const uint32_t num = 10000;

void func1(void* p)
{
  for(uint32_t i=0; i<num; ++i)
  {
    cout << 1 << endl;
  }
}

void func2(void* p)
{
  for(uint32_t i=0; i<num; ++i)
  {
    cout << 2 << endl;
  }
}

struct CustomEvent : public lost::event::Event
{
  CustomEvent() : Event("custom")
  {
  }
};

struct Target
{
  void handleEvent(const lost::event::Event* ev)
  {
    DOUT("got event: "<<ev->type);
  }
};

struct OtherTarget
{
  void handleEvent(const CustomEvent* ev)
  {
    DOUT("got event: "<<ev->type);
  }
};


int main (int argc, char*  argv[]) {
/*  cout << "hardware concurrency "<<tthread::thread::hardware_concurrency()<<endl;
  tthread::thread t1(func1, NULL);
  tthread::thread t2(func2, NULL);
  t1.join();
  t2.join();*/
  
/*  Target target;
  OtherTarget otherTarget;
  lost::event::TypedListener<Target,lost::event::Event> listener(&target, &Target::handleEvent);
  lost::event::TypedListener<OtherTarget,CustomEvent> listener2(&otherTarget, &OtherTarget::handleEvent);
  lost::event::Event ev;
  CustomEvent ev2;
  listener.call(&ev);
  listener.call(&ev2);
  
  eastl::string s = "hello";
  DOUT(s);*/
  lost::application::runResourceTasklet(argc, argv, "Launcher.tasklet");
  
  return EXIT_SUCCESS;
}
