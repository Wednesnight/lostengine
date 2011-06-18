#include <iostream>
#include "lost/event/EventDispatcher.h"
#include "lost/event/Listener.h"
#include "lost/event/Pool.h"
#include "lost/event/Event.h"
#include "lost/application/MouseEvent.h"
#include <sys/time.h>

using namespace std;
using namespace lost;
struct Target
{
  void handleEvent(const event::EventPtr& ev)
  {
    cout << ev->type << endl;
  }
};

    string currentTimeFormat()
    {
      const size_t bufsize = 30;
      char   timeformat[bufsize];

      struct timeval tv;
      gettimeofday(&tv, NULL);

      strftime( timeformat, bufsize, "%Y/%m/%d %H:%M:%S", localtime(&tv.tv_sec));
      timeformat[19] = '.';
      timeformat[20] = '%';
      timeformat[21] = '0';
      timeformat[22] = '3';
      timeformat[23] = 'd';
      timeformat[24] = 0;
      snprintf(timeformat, bufsize, timeformat, tv.tv_usec/1000);

      return timeformat;
    }

int main (int argc, char * const argv[]) {
    // insert code here...
//    Target target;
//    event::EventDispatcher ed;
//    ed.addEventListener("hello", event::makeListener(&target, &Target::handleEvent));
//    ed.dispatchEvent(event::Event::create("hello"));
    event::Pool* pool = event::Pool::instance();

/*    event::TypedHandle<event::Event> t1;
    event::TypedHandle<application::MouseEvent> t2;
    
    {
    event::TypedHandle<event::Event> ev1 = pool->createEvent<event::Event>("randomEvent");
    t1 = ev1;
    event::TypedHandle<event::Event> ev2 = pool->createEvent<event::Event>("helloEvent");
    event::TypedHandle<application::MouseEvent> ev3 = pool->createEvent<application::MouseEvent>("mouseUp");
    t2 = ev3;
    event::TypedHandle<application::MouseEvent> ev4 = pool->createEvent<application::MouseEvent>("mouseDown");
    event::TypedHandle<application::MouseEvent> ev5 = pool->createEvent<application::MouseEvent>("mouseMove");

    cout << ev1->type << endl;
    cout << ev2->type << endl;
    cout << ev3->type << endl;
    cout << ev4->type << endl;
    cout << ev5->type << endl;
    }
    
    {
      event::TypedHandle<event::Event> ev1 = pool->createEvent<event::Event>("randomEvent");
      event::TypedHandle<application::MouseEvent> ev4 = pool->createEvent<application::MouseEvent>("mouseDown");
    }*/
    
    typedef event::TypedHandle<event::Event> EventHandle;
    vector<EventHandle> v;
    const uint32_t N = 100000;
    cout << currentTimeFormat()<< " container warmup with empty handles" << endl;
    for(uint32_t i=0; i<N; ++i)
    {
      v.push_back(EventHandle());
    }
    cout << currentTimeFormat() << " fill with event handles from pool" << endl;
    for(uint32_t i=0; i<N; ++i)
    {
      v.push_back(pool->createEvent<event::Event>("hello"));
    }
    cout << currentTimeFormat() << " clearing" << endl;
    v.clear();
    cout << currentTimeFormat()<< " DONE" << endl;
    return 0;
}
