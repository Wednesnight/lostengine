#include <iostream>
#include "lost/event/EventDispatcher.h"
#include "lost/event/Listener.h"
#include "lost/event/Pool.h"
#include "lost/event/Event.h"

using namespace std;
using namespace lost;
struct Target
{
  void handleEvent(const event::EventPtr& ev)
  {
    cout << ev->type << endl;
  }
};

int main (int argc, char * const argv[]) {
    // insert code here...
    Target target;
    event::EventDispatcher ed;
    ed.addEventListener("hello", event::makeListener(&target, &Target::handleEvent));
    ed.dispatchEvent(event::Event::create("hello"));
    event::Pool* pool = event::Pool::instance();

    event::TypedHandle<event::Event> ev1 = pool->createEvent<event::Event>("randomEvent");
    cout << ev1->type << endl;
    
    return 0;
}
