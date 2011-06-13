#include "lost/event/Event.h"
#include "lost/event/Listener.h"
#include "lost/event/Signal.h"
#include <EASTL/string.h>
#include "lost/common/Logger.h"

#include "lost/application/Runner.h"

#include "lost/event/EventDispatcher.h"

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
  void handleEvent(const lost::event::EventPtr& ev)
  {
    DOUT("got event: "<<ev->type);
  }
};

typedef lost::shared_ptr<CustomEvent> CustomEventPtr;

struct OtherTarget
{
  void handleEvent(const CustomEventPtr& ev)
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
  
  Target target;
  OtherTarget otherTarget;
/*  lost::event::TypedListener<Target,lost::event::Event> listener(&target, &Target::handleEvent);
  lost::event::TypedListener<OtherTarget,CustomEvent> listener2(&otherTarget, &OtherTarget::handleEvent);
  lost::event::Event ev;
  CustomEvent ev2;
  listener.call(&ev);
  listener.call(&ev2);
  
  eastl::string s = "hello";
  DOUT(s);*/
  
  lost::event::EventDispatcher evd;
  lost::event::ListenerPtr listener(new lost::event::TypedListener<Target, lost::event::Event>(&target, &Target::handleEvent));
  lost::event::ListenerPtr listener2(new lost::event::TypedListener<OtherTarget, CustomEvent>(&otherTarget, &OtherTarget::handleEvent));
  evd.addEventListener("hello", listener);
  evd.addEventListener("hello2", listener2);
  
  using namespace lost::event;
  
  EventPtr ev1(new Event("hello"));
  EventPtr ev2(new Event("hello2"));
  EventPtr ev3(new Event("bluuuu"));
  
  evd.dispatchEvent(ev1);
  evd.dispatchEvent(ev2);
  evd.dispatchEvent(ev3);
  
  lost::application::runResourceTasklet(argc, argv, "Launcher.tasklet");
  
  return EXIT_SUCCESS;
}
