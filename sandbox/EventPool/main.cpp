#include <iostream>
#include "lost/event/EventDispatcher.h"
#include "lost/event/Listener.h"
#include "lost/event/Pool.h"
#include "lost/event/Event.h"
#include "lost/application/MouseEvent.h"

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
    event::TypedHandle<event::Event> ev2 = pool->createEvent<event::Event>("helloEvent");
    event::TypedHandle<application::MouseEvent> ev3 = pool->createEvent<application::MouseEvent>("mouseUp");
    event::TypedHandle<application::MouseEvent> ev4 = pool->createEvent<application::MouseEvent>("mouseDown");
    event::TypedHandle<application::MouseEvent> ev5 = pool->createEvent<application::MouseEvent>("mouseMove");

    cout << ev1->type << endl;
    cout << ev2->type << endl;
    cout << ev3->type << endl;
    cout << ev4->type << endl;
    cout << ev5->type << endl;
    
    return 0;
}
